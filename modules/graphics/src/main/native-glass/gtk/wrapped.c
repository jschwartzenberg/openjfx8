/*
 * Copyright (c) 2018, Oracle and/or its affiliates. All rights reserved.
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

#include <stdio.h>
#include <linux/fb.h>
#include <fcntl.h>
#ifndef __USE_GNU       // required for dladdr() & Dl_info
#define __USE_GNU
#endif
#include <dlfcn.h>
#include <sys/ioctl.h>

#include <jni.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "wrapped.h"

extern jboolean gtk_verbose;

/*
 * cpp and dlsym don't play nicely together. Do all dynamic loading in C
 */


static void (*_gdk_x11_display_set_window_scale) (GdkDisplay *display, gint scale);

// Note added in libgdk 3.10 which is > our OEL 7.0 version of 3.8
void wrapped_gdk_x11_display_set_window_scale (GdkDisplay *display,
                                  gint scale)
{
#if GTK_CHECK_VERSION(3, 0, 0)
    if(_gdk_x11_display_set_window_scale == NULL) {
        _gdk_x11_display_set_window_scale = dlsym(RTLD_DEFAULT, "gdk_x11_display_set_window_scale");
        if (gtk_verbose && _gdk_x11_display_set_window_scale) {
            fprintf(stderr, "loaded gdk_x11_display_set_window_scale\n"); fflush(stderr);
        }
    }
#endif

    if(_gdk_x11_display_set_window_scale != NULL) {
        (*_gdk_x11_display_set_window_scale)(display, scale);
    }
}

