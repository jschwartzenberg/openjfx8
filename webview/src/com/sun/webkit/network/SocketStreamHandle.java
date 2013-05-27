/*
 * Copyright (c) 2012, Oracle and/or its affiliates. All rights reserved.
 */
package com.sun.webkit.network;

import com.sun.webkit.Invoker;
import com.sun.webkit.WebPage;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import static java.lang.String.format;
import java.net.ConnectException;
import java.net.InetSocketAddress;
import java.net.NoRouteToHostException;
import java.net.PortUnreachableException;
import java.net.Proxy;
import java.net.ProxySelector;
import java.net.Socket;
import java.net.SocketException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.UnknownHostException;
import java.security.AccessController;
import java.security.PrivilegedAction;
import java.security.PrivilegedActionException;
import java.security.PrivilegedExceptionAction;
import java.util.List;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLException;
import javax.net.ssl.SSLSocket;

final class SocketStreamHandle {
    private static final Pattern FIRST_LINE_PATTERN = Pattern.compile(
            "^HTTP/1.[01]\\s+(\\d{3})(?:\\s.*)?$");
    private static final Logger logger = Logger.getLogger(
            SocketStreamHandle.class.getName());
    private static final ThreadPoolExecutor threadPool = new ThreadPoolExecutor(
            0, Integer.MAX_VALUE,
            10, TimeUnit.SECONDS,
            new SynchronousQueue<Runnable>(),
            new CustomThreadFactory());

    private enum State {ACTIVE, CLOSE_REQUESTED, DISPOSED}

    private final String host;
    private final int port;
    private final boolean ssl;
    private final WebPage webPage;
    private final long data;
    private volatile Socket socket;
    private volatile State state = State.ACTIVE;
    private volatile boolean connected;

    private SocketStreamHandle(String host, int port, boolean ssl,
                               WebPage webPage, long data)
    {
        this.host = host;
        this.port = port;
        this.ssl = ssl;
        this.webPage = webPage;
        this.data = data;
    }

    private static SocketStreamHandle fwkCreate(String host, int port,
                                                boolean ssl, WebPage webPage,
                                                long data)
    {
        final SocketStreamHandle ssh =
                new SocketStreamHandle(host, port, ssl, webPage, data);
        logger.log(Level.FINEST, "Starting {0}", ssh);
        threadPool.submit(new Runnable() {
            @Override public void run() {
                ssh.run();
            }
        });
        return ssh;
    }

    private void run() {
        if (webPage == null) {
            logger.log(Level.FINEST, "{0} is not associated with any web "
                    + "page, aborted", this);
            // In theory we could pump this error through the doRun()'s
            // error handling code but in that case that error handling
            // code would have to run outside the doPrivileged block,
            // which is something we want to avoid.
            didFail(0, "Web socket is not associated with any web page");
            didClose();
            return;
        }
        AccessController.doPrivileged(new PrivilegedAction<Void>() {
            @Override public Void run() {
                doRun();
                return null;
            }
        }, webPage.getAccessControlContext());
    }

    private void doRun() {
        Throwable error = null;
        String errorDescription = null;
        try {
            logger.log(Level.FINEST, "{0} started", this);
            connect();
            connected = true;
            logger.log(Level.FINEST, "{0} connected", this);
            didOpen();
            InputStream is = socket.getInputStream();
            while (true) {
                byte[] buffer = new byte[8192];
                int n = is.read(buffer);
                if(n > 0) {
                    if (logger.isLoggable(Level.FINEST)) {
                        logger.log(Level.FINEST, format("%s received len: [%d],"
                                + " data:%s", this, n, dump(buffer, n)));
                    }
                    didReceiveData(buffer, n);
                } else {
                    logger.log(Level.FINEST, "{0} connection closed by "
                            + "remote host", this);
                    break;
                }
            }
        } catch (UnknownHostException ex) {
            error = ex;
            errorDescription = "Unknown host";
        } catch (ConnectException ex) {
            error = ex;
            errorDescription = "Unable to connect";
        } catch (NoRouteToHostException ex) {
            error = ex;
            errorDescription = "No route to host";
        } catch (PortUnreachableException ex) {
            error = ex;
            errorDescription = "Port unreachable";
        } catch (SocketException ex) {
            if (state != State.ACTIVE) {
                if (logger.isLoggable(Level.FINEST)) {
                    logger.log(Level.FINEST, format("%s exception (most "
                            + "likely caused by local close)", this), ex);
                }
            } else {
                error = ex;
                errorDescription = "Socket error";
            }
        } catch (SSLException ex) {
            error = ex;
            errorDescription = "SSL error";
        } catch (IOException ex) {
            error = ex;
            errorDescription = "I/O error";
        } catch (SecurityException ex) {
            error = ex;
            errorDescription = "Security error";
        } catch (Throwable th) {
            error = th;
        }

        if (error != null) {
            if (errorDescription == null) {
                errorDescription = "Unknown error";
                logger.log(Level.WARNING, format("%s unexpected error", this),
                           error);
            } else {
                logger.log(Level.FINEST, format("%s exception", this), error);
            }
            didFail(0, errorDescription);
        }

        try {
            socket.close();
        } catch (IOException ignore) {}
        didClose();

        logger.log(Level.FINEST, "{0} finished", this);
    }

    private void connect() throws IOException {
        SecurityManager securityManager = System.getSecurityManager();
        if (securityManager != null) {
            securityManager.checkConnect(host, port);
        }

        // The proxy trial logic here is meant to mimic
        // sun.net.www.protocol.http.HttpURLConnection.plainConnect
        boolean success = false;
        IOException lastException = null;
        boolean triedDirectConnection = false;
        ProxySelector proxySelector = AccessController.doPrivileged(
                new PrivilegedAction<ProxySelector>() {
                    public ProxySelector run() {
                        return ProxySelector.getDefault();
                    }
                });
        if (proxySelector != null) {
            URI uri;
            try {
                uri = new URI((ssl ? "https" : "http") + "://" + host);
            } catch (URISyntaxException ex) {
                throw new IOException(ex);
            }
            if (logger.isLoggable(Level.FINEST)) {
                logger.log(Level.FINEST, format("%s selecting proxies "
                        + "for: [%s]", this, uri));
            }
            List<Proxy> proxies = proxySelector.select(uri);
            if (logger.isLoggable(Level.FINEST)) {
                logger.log(Level.FINEST, format("%s selected proxies: %s",
                        this, proxies));
            }
            for (Proxy proxy : proxies) {
                if (logger.isLoggable(Level.FINEST)) {
                    logger.log(Level.FINEST, format("%s trying proxy: [%s]",
                            this, proxy));
                }
                if (proxy.type() == Proxy.Type.DIRECT) {
                    triedDirectConnection = true;
                }
                try {
                    connect(proxy);
                    success = true;
                    break;
                } catch (IOException ex) {
                    logger.log(Level.FINEST, format("%s exception", this), ex);
                    lastException = ex;
                    if (proxy.address() != null) {
                        proxySelector.connectFailed(uri, proxy.address(), ex);
                    }
                    continue;
                }
            }
        }
        if (!success && !triedDirectConnection) {
            logger.log(Level.FINEST, "{0} trying direct connection", this);
            connect(Proxy.NO_PROXY);
            success = true;
        }
        if (!success) {
            throw lastException;
        }
    }

    private void connect(Proxy proxy) throws IOException {
        if (proxy.type() == Proxy.Type.HTTP) {
            synchronized (this) {
                if (state != State.ACTIVE) {
                    throw new SocketException("Close requested");
                }
                socket = new Socket(Proxy.NO_PROXY);
            }
            if (logger.isLoggable(Level.FINEST)) {
                logger.log(Level.FINEST, format("%s connecting to proxy: [%s]",
                        this, proxy));
            }
            final InetSocketAddress address =
                    (InetSocketAddress) proxy.address();
            try {
                AccessController.doPrivileged(
                    new PrivilegedExceptionAction<Void>() {
                        @Override public Void run() throws IOException {
                            socket.connect(new InetSocketAddress(
                                    address.getHostName(),
                                    address.getPort()));
                            return null;
                        }
                    });
            } catch (PrivilegedActionException ex) {
                throw (IOException) ex.getException();
            }
            if (logger.isLoggable(Level.FINEST)) {
                logger.log(Level.FINEST, format("%s connected to proxy: [%s]",
                        this, proxy));
            }
            setupProxyTunnel();
        } else { // DIRECT or SOCKS
            synchronized (this) {
                if (state != State.ACTIVE) {
                    throw new SocketException("Close requested");
                }
                socket = new Socket(proxy);
            }
            if (logger.isLoggable(Level.FINEST)) {
                logger.log(Level.FINEST, format("%s connecting to: [%s:%d]",
                        this, host, port));
            }
            socket.connect(new InetSocketAddress(host, port));
            if (logger.isLoggable(Level.FINEST)) {
                logger.log(Level.FINEST, format("%s connected to: [%s:%d]",
                        this, host, port));
            }
        }
        if (ssl) {
            synchronized (this) {
                if (state != State.ACTIVE) {
                    throw new SocketException("Close requested");
                }
                logger.log(Level.FINEST, "{0} starting SSL handshake", this);
                socket = HttpsURLConnection.getDefaultSSLSocketFactory()
                        .createSocket(socket, host, port, true);
            }
            ((SSLSocket) socket).startHandshake();
        }
    }

    private void setupProxyTunnel() throws IOException {
        logger.log(Level.FINEST, "{0} setting up proxy tunnel", this);

        String request =
                "CONNECT " + host + ":" + port + " HTTP/1.1\r\n"
                + "Host: " + host + "\r\n"
                + "Proxy-Connection: keep-alive\r\n"
                + "\r\n";
        if (logger.isLoggable(Level.FINEST)) {
            logger.log(Level.FINEST, format("%s sending:%n%s",
                    this, request.replaceAll("(?m)^", "    ")));
        }
        OutputStreamWriter w =
                new OutputStreamWriter(socket.getOutputStream(), "US-ASCII");
        w.write(request);
        w.flush();

        String firstLine = null;
        StringBuilder line = new StringBuilder();
        InputStreamReader r =
                new InputStreamReader(socket.getInputStream(), "UTF-8");
        while (true) {
            int c = r.read();
            if (c < 0) {
                if (logger.isLoggable(Level.FINEST)) {
                    logger.log(Level.FINEST, format("%s received: [%s]",
                            this, line));
                }
                throw new SocketException("Connection closed by proxy "
                        + "during tunnel setup");
            } else if (c == '\n') {
                if (line.length() > 0
                        && line.charAt(line.length() - 1) == '\r')
                {
                    line.deleteCharAt(line.length() - 1);
                }
                if (logger.isLoggable(Level.FINEST)) {
                    logger.log(Level.FINEST, format("%s received: [%s]",
                            this, line));
                }
                if (line.length() == 0) {
                    break; // empty line indicates end of headers
                } else {
                    if (firstLine == null) {
                        firstLine = line.toString();
                    }
                    line.setLength(0);
                }
            } else {
                line.append((char) c);
            }
        }

        if (firstLine == null) {
            throw new SocketException("Empty response from proxy "
                    + "during tunnel setup");
        }

        Matcher matcher = FIRST_LINE_PATTERN.matcher(firstLine);
        if (!matcher.matches()) {
            throw new SocketException("Unexpected response from proxy during "
                    + "tunnel setup: [" + firstLine + "]");
        }

        int responseCode = Integer.parseInt(matcher.group(1));
        if (responseCode != 200) {
            // TODO: handle proxy authentication RT-25644
            throw new SocketException("Error setting up proxy tunnel: "
                    + "[" + firstLine + "]");
        }

        logger.log(Level.FINEST, "{0} proxy tunnel set up successfully", this);
    }

    private int fwkSend(byte[] buffer) {
        if (logger.isLoggable(Level.FINEST)) {
            logger.log(Level.FINEST, format("%s sending len: [%d], data:%s",
                    this, buffer.length, dump(buffer, buffer.length)));
        }
        if (connected) {
            try {
                socket.getOutputStream().write(buffer);
                return buffer.length;
            } catch (IOException ex) {
                logger.log(Level.FINEST, format("%s exception", this), ex);
                didFail(0, "I/O error");
                return 0;
            }
        } else {
            logger.log(Level.FINEST, "{0} not connected", this);
            didFail(0, "Not connected");
            return 0;
        }
    }

    private void fwkClose() {
        synchronized (this) {
            logger.log(Level.FINEST, "{0}", this);
            state = State.CLOSE_REQUESTED;
            try {
                if (socket != null) {
                    socket.close();
                }
            } catch (IOException ignore) {}
        }
    }

    private void fwkNotifyDisposed() {
        logger.log(Level.FINEST, "{0}", this);
        state = State.DISPOSED;
    }

    private void didOpen() {
        Invoker.getInvoker().postOnEventThread(new Runnable() {
            @Override public void run() {
                if (state == State.ACTIVE) {
                    notifyDidOpen();
                }
            }
        });
    }

    private void didReceiveData(final byte[] buffer, final int len) {
        Invoker.getInvoker().postOnEventThread(new Runnable() {
            @Override public void run() {
                if (state == State.ACTIVE) {
                    notifyDidReceiveData(buffer, len);
                }
            }
        });
    }

    private void didFail(final int errorCode, final String errorDescription) {
        Invoker.getInvoker().postOnEventThread(new Runnable() {
            @Override public void run() {
                if (state == State.ACTIVE) {
                    notifyDidFail(errorCode, errorDescription);
                }
            }
        });
    }

    private void didClose() {
        Invoker.getInvoker().postOnEventThread(new Runnable() {
            @Override public void run() {
                if (state != State.DISPOSED) {
                    notifyDidClose();
                }
            }
        });
    }

    private void notifyDidOpen() {
        logger.log(Level.FINEST, "{0}", this);
        twkDidOpen(data);
    }

    private void notifyDidReceiveData(byte[] buffer, int len) {
        if (logger.isLoggable(Level.FINEST)) {
            logger.log(Level.FINEST, format("%s, len: [%d], data:%s",
                    this, len, dump(buffer, len)));
        }
        twkDidReceiveData(buffer, len, data);
    }

    private void notifyDidFail(int errorCode, String errorDescription) {
        if (logger.isLoggable(Level.FINEST)) {
            logger.log(Level.FINEST, format("%s, errorCode: %d, "
                    + "errorDescription: %s",
                    this, errorCode, errorDescription));
        }
        twkDidFail(errorCode, errorDescription, data);
    }

    private void notifyDidClose() {
        logger.log(Level.FINEST, "{0}", this);
        twkDidClose(data);
    }

    private static native void twkDidOpen(long data);
    private static native void twkDidReceiveData(byte[] buffer, int len,
                                                 long data);
    private static native void twkDidFail(int errorCode,
                                          String errorDescription, long data);
    private static native void twkDidClose(long data);

    private static String dump(byte[] buffer, int len) {
        StringBuilder sb = new StringBuilder();
        int i = 0;
        while (i < len) {
            StringBuilder c1 = new StringBuilder();
            StringBuilder c2 = new StringBuilder();
            for (int k = 0; k < 16; k++, i++) {
                if (i < len) {
                    int b = buffer[i] & 0xff;
                    c1.append(format("%02x ", b));
                    c2.append((b >= 0x20 && b <= 0x7e) ? (char) b : '.');
                } else {
                    c1.append("   ");
                }
            }
            sb.append(format("%n  ")).append(c1).append(' ').append(c2);
        }
        return sb.toString();
    }

    @Override
    public String toString() {
        return format("SocketStreamHandle{host=%s, port=%d, ssl=%s, "
                + "data=0x%016X, state=%s, connected=%s}",
                host, port, ssl, data, state, connected);
    }

    private static final class CustomThreadFactory implements ThreadFactory {
        private final ThreadGroup group;
        private final AtomicInteger index = new AtomicInteger(1);

        private CustomThreadFactory() {
            SecurityManager sm = System.getSecurityManager();
            group = (sm != null) ? sm.getThreadGroup()
                    : Thread.currentThread().getThreadGroup();
        }

        @Override
        public Thread newThread(Runnable r) {
            Thread t = new Thread(group, r, "SocketStreamHandle-"
                    + index.getAndIncrement());
            t.setDaemon(true);
            if (t.getPriority() != Thread.NORM_PRIORITY) {
                t.setPriority(Thread.NORM_PRIORITY);
            }
            return t;
        }
    }
}
