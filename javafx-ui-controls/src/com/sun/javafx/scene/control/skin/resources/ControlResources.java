/*
 * Copyright (c) 2011, 2013, Oracle and/or its affiliates. All rights reserved.
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

package com.sun.javafx.scene.control.skin.resources;

import java.util.ResourceBundle;

public final class ControlResources {

    private static final String BASE_NAME = "com/sun/javafx/scene/control/skin/resources/controls";

    // Do not cache the bundle here. It is cached by the ResourceBundle
    // class and may be updated if the default locale changes.

    /*
     * Look up a string in the properties file corresponding to the
     * default locale (i.e. the application's locale). If not found, the
     * search then falls back to the base controls.properties file,
     * containing the default string (usually English).
     */
    public static String getString(String key) {
        return ResourceBundle.getBundle(BASE_NAME).getString(key);
    }
}
