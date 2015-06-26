/*
 * Copyright (c) 2013, 2015, Oracle and/or its affiliates. All rights reserved.
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

package com.sun.webkit;

import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import com.oracle.dalvik.InternalWebView;

public class NativeWebView {

    private static List<NativeWebView> views = new ArrayList<NativeWebView>();
    private int id;
    private WebPage page;

    public NativeWebView(WebPage page) {
        id = _createAndroidWebView();
        this.page = page;
        views.add(this);
    }

    public void moveToTop() {
        _moveToTop(this.id);
    }

    public void moveAndResize(int x, int y, int width, int height) {
        _moveAndResize(this.id, x, y, width, height);
    }

    public void setVisible(boolean visible) {
        _setVisible(this.id, visible);
    }

    void loadUrl(String url) {
        _loadUrl(this.id, url);
    }

    void loadContent(String content, String contentType) {
        _loadContent(this.id, content, contentType);
    }

    void setEncoding(String encoding) {
        _setEncoding(this.id, encoding);
    }

    void dispose() {        
        _dispose(this.id);
        views.remove(this);
    }

    String getHtmlContent() {
        return InternalWebView.getHtmlContent(this.id);
    }

    private static NativeWebView getViewByID(int id) {
        for (NativeWebView wvp : views) {
            if (id == wvp.id) {
                return wvp;
            }
        }
        System.err.println("Accesing nonexisting/disposed NativewWebView id: " + id);
        return null;
    }

    public static void fire_load_event(final int id, final int frameID, final int state,
            final String url, final String contenType,
            final int progress, final int errorCode) {
        final NativeWebView nwv = NativeWebView.getViewByID(id);
        if (nwv == null) {            
            return;
        }
        Invoker.getInvoker().invokeOnEventThread(new Runnable() {
            @Override
            public void run() {
                double dprogress = progress / 100.0;
                nwv.page.fireLoadEvent(frameID, state, url, contenType, dprogress, errorCode);
            }
        });
    }

    static Map<Integer, InternalWebView> webViews = new HashMap<>();

    // private native void _moveAndResize(int id, int x, int y, int width, int height);
    private void _moveAndResize(int id, int x, int y, int width, int height) {
        InternalWebView.moveAndResize(id, x, y, width, height);
    }

    // private native void _setVisible(int id, boolean visible);
    private void _setVisible(int id, boolean visible) {
        InternalWebView.setVisible(id, visible);
    }

    // private native int _createAndroidWebView();
    private int _createAndroidWebView() {
        InternalWebView internalWebView = new InternalWebView();
        int id = internalWebView.getInternalID();
        webViews.put(id, internalWebView);
        return id;
    }


    // private native void _moveToTop(int id);
    private void _moveToTop(int id) {
    }

    // private native void _loadUrl(int id, String url);
    private void _loadUrl(int id, String url) {
        InternalWebView.loadUrl(id, url);
        // InternalWebView target = webViews.get(id);
        // target.loadUrl(url);

    }

    // private native void _dispose(int id);
    private void _dispose(int id) {
        InternalWebView.dispose(id);
    }

    // private native void _loadContent(int id, String content, String contentType);
    private void _loadContent(int id, String content, String contentType) {
        InternalWebView.loadContent(id, content, contentType);
    }

    private native void _setEncoding(int id, String encoding);
}
