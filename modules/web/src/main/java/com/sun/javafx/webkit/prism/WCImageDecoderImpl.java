/*
 * Copyright (c) 2011, 2014, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

package com.sun.javafx.webkit.prism;

import com.sun.javafx.iio.ImageFrame;
import com.sun.javafx.iio.ImageLoadListener;
import com.sun.javafx.iio.ImageLoader;
import com.sun.javafx.iio.ImageMetadata;
import com.sun.javafx.iio.ImageStorage;
import com.sun.javafx.iio.ImageStorageException;
import com.sun.webkit.graphics.WCGraphicsManager;
import com.sun.webkit.graphics.WCImage;
import com.sun.webkit.graphics.WCImageDecoder;
import com.sun.webkit.graphics.WCImageFrame;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.concurrent.Service;
import javafx.concurrent.Task;

final class WCImageDecoderImpl extends WCImageDecoder {

    private final static Logger log;

    private Service<ImageFrame[]> loader;

    private int imageWidth = 0;
    private int imageHeight = 0;
    private ImageFrame[] frames;
    private int frameCount = 0; // keeps frame count when decoded frames are temporarily destroyed
    private boolean fullDataReceived = false;
    private boolean framesDecoded = false; // guards frames from repeated decoding
    private PrismImage[] images;
    private volatile byte[] data;
    private volatile int dataSize = 0;

    static {
        log = Logger.getLogger(WCImageDecoderImpl.class.getName());
    }

    /*
     * This method is supposed to be called from ImageSource::clear() method
     * when either the decoded data or the image decoder itself are to be destroyed.
     * It should free all complex object on the java layer and explicitely
     * destroy objects which has native resources.
     */
    @Override protected void destroy() {
        if (log.isLoggable(Level.FINE)) {
            log.fine(String.format("%X Destroy image decoder", hashCode()));
        }

        destroyLoader();
        frames = null;
        images = null;
        framesDecoded = false;
    }

    @Override protected String getFilenameExtension() {
        /// retrieve image format from reader
        return ".img";
    }

    private boolean imageSizeAvilable() {
        return imageWidth > 0 && imageHeight > 0;
    }

    @Override protected void addImageData(byte[] dataPortion) {
        if (dataPortion != null) {
            fullDataReceived = false;
            if (data == null) {
                data = Arrays.copyOf(dataPortion, dataPortion.length * 2);
                dataSize = dataPortion.length;
            } else {
                int newDataSize = dataSize + dataPortion.length;
                if (newDataSize > data.length) {
                    resizeDataArray(Math.max(newDataSize, data.length * 2));
                }
                System.arraycopy(dataPortion, 0, data, dataSize, dataPortion.length);
                dataSize = newDataSize;
            }
            // Try to decode the partial data until we get image size.
            if (!imageSizeAvilable()) {
                loadFrames();
            }
        } else if (data != null && !fullDataReceived) {
            // null dataPortion means data completion
            if (data.length > dataSize) {
                resizeDataArray(dataSize);
            }
            fullDataReceived = true;
        }
    }

    private void destroyLoader() {
        if (loader != null) {
            loader.cancel();
            loader = null;
        }
    }

    private void startLoader() {
        if (this.loader == null) {
            this.loader = new Service<ImageFrame[]>() {
                protected Task<ImageFrame[]> createTask() {
                    return new Task<ImageFrame[]>() {
                        protected ImageFrame[] call() throws Exception {
                            return loadFrames();
                        }
                    };
                }
            };
            this.loader.valueProperty().addListener((ov, old, frames) -> {
                if ((frames != null) && (loader != null)) {
                    setFrames(frames);
                }
            });
        }
        if (!this.loader.isRunning()) {
            this.loader.restart();
        }
    }

    private void resizeDataArray(int newDataSize) {
        byte[] newData = new byte[newDataSize];
        System.arraycopy(data, 0, newData, 0, dataSize);
        data = newData;
    }

    @Override protected void loadFromResource(String name) {
        if (log.isLoggable(Level.FINE)) {
            log.fine(String.format(
                    "%X Load image from resource '%s'", hashCode(), name));
        }

        String resourceName = WCGraphicsManager.getResourceName(name);
        InputStream in = getClass().getResourceAsStream(resourceName);
        if (in == null) {
            if (log.isLoggable(Level.FINE)) {
                log.fine(String.format(
                        "%X Unable to open resource '%s'", hashCode(), resourceName));
            }
            return;
        }

        setFrames(loadFrames(in));
    }

    private ImageFrame[] loadFrames(InputStream in) {
        if (log.isLoggable(Level.FINE)) {
            log.fine(String.format("%X Decoding frames", hashCode()));
        }
        try {
            return ImageStorage.loadAll(in, readerListener, 0, 0, true, 1.0f, false);
        } catch (ImageStorageException e) {
            return null; // consider image missing
        } finally {
            try {
                in.close();
            } catch (IOException e) {
                // ignore
            }
        }
    }

    private ImageFrame[] loadFrames() {
        return loadFrames(new ByteArrayInputStream(this.data, 0, this.dataSize));
    }

    private final ImageLoadListener readerListener = new ImageLoadListener() {
        @Override public void imageLoadProgress(ImageLoader l, float p) {
        }
        @Override public void imageLoadWarning(ImageLoader l, String warning) {
        }
        @Override public void imageLoadMetaData(ImageLoader l, ImageMetadata metadata) {
            if (log.isLoggable(Level.FINE)) {
                log.fine(String.format("%X Image size %dx%d",
                        hashCode(), metadata.imageWidth, metadata.imageHeight));
            }
            // The following lines is a workaround for RT-13475,
            // because image decoder does not report valid image size
            if (imageWidth < metadata.imageWidth) {
                imageWidth = metadata.imageWidth;
            }
            if (imageHeight < metadata.imageHeight) {
                imageHeight = metadata.imageHeight;
            }
        }
    };

    @Override protected void getImageSize(int[] size) {
        size[0] = imageWidth;
        size[1] = imageHeight;
        if (log.isLoggable(Level.FINE)) {
            log.fine(String.format("%X image size = %dx%d", hashCode(), size[0], size[1]));
        }
    }

    private static final class Frame extends WCImageFrame {
        private WCImage image;

        private Frame(WCImage image) {
            this.image = image;
        }

        @Override public WCImage getFrame() {
            return image;
        }

        @Override protected void destroyDecodedData() {
            image = null;
        }
    }

    private void setFrames(ImageFrame[] frames) {
        this.frames = frames;
        this.images = null;
        frameCount = frames == null ? 0 : frames.length;
    }

    @Override protected int getFrameCount() {
        return frameCount;
    }

    @Override protected WCImageFrame getFrame(int idx, int[] data) {
        ImageFrame frame = getImageFrame(idx);
        if (frame != null) {
            if (log.isLoggable(Level.FINE)) {
                ImageStorage.ImageType type = frame.getImageType();
                log.fine(String.format("%X getFrame(%d): image type = %s",
                        hashCode(), idx, type));
            }
            PrismImage img = getPrismImage(idx, frame);

            if (data != null) {
                ImageMetadata meta = frame.getMetadata();
                int dur = (meta == null || meta.delayTime == null) ? 0 : meta.delayTime;
                // Many annoying ads try to animate too fast.
                // See RT-13535 or <http://webkit.org/b/36082>.
                if (dur < 11) dur = 100;

                data[0] = (idx < frames.length - 1) ? 1 : 0;
                data[1] = img.getWidth();
                data[2] = img.getHeight();
                data[3] = dur;
                data[4] = 1;  /// hasAlpha

                if (log.isLoggable(Level.FINE)) {
                    log.fine(String.format(
                            "%X getFrame(%d): complete=%d, size=%dx%d, duration=%d, hasAlpha=%d",
                            hashCode(), idx, data[0], data[1], data[2], data[3], data[4]));
                }
            }
            return new Frame(img);
        }
        if (log.isLoggable(Level.FINE)) {
            log.fine(String.format("%X FAILED getFrame(%d)", hashCode(), idx));
        }
        return null;
    }

    private ImageFrame getImageFrame(int idx) {
        if (!fullDataReceived) {
            startLoader();
        } else if (fullDataReceived && !framesDecoded) {
            destroyLoader();
            setFrames(loadFrames()); // re-decode frames if they have been destroyed
            framesDecoded = true;
        }
        return (idx >= 0) && (this.frames != null) && (this.frames.length > idx)
                ? this.frames[idx]
                : null;
    }

    private PrismImage getPrismImage(int idx, ImageFrame frame) {
        if (this.images == null) {
            this.images = new PrismImage[this.frames.length];
        }
        if (this.images[idx] == null) {
            this.images[idx] = new WCImageImpl(frame);
        }
        return this.images[idx];
    }
}
