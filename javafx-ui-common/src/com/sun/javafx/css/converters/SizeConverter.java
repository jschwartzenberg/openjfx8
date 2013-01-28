/*
 * Copyright (c) 2011, 2012, Oracle and/or its affiliates. All rights reserved.
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
package com.sun.javafx.css.converters;

import com.sun.javafx.css.Size;
import com.sun.javafx.css.StyleConverterImpl;
import javafx.css.ParsedValue;
import javafx.css.StyleConverter;
import javafx.scene.text.Font;

/* Convert a Size to Number */
public final class SizeConverter extends StyleConverterImpl<ParsedValue<?, Size>, Number> {

    // lazy, thread-safe instatiation
    private static class Holder {
        static SizeConverter INSTANCE = new SizeConverter();
        static SequenceConverter SEQUENCE_INSTANCE = new SequenceConverter();
    }

    public static StyleConverter<ParsedValue<?, Size>, Number> getInstance() {
        return Holder.INSTANCE;
    }

    private SizeConverter() {
        super();
    }

    @Override
    public Number convert(ParsedValue<ParsedValue<?, Size>, Number> value, Font font) {
        ParsedValue<?, Size> size = value.getValue();
        return size.convert(font).pixels(font);
    }

    @Override
    public String toString() {
        return "SizeConverter";
    }

    /*
     * Convert [<size>]+ to an array of Number[].
     */
    public static final class SequenceConverter extends StyleConverterImpl<ParsedValue[], Number[]> {

        public static SequenceConverter getInstance() {
            return Holder.SEQUENCE_INSTANCE;
        }

        private SequenceConverter() {
            super();
        }

        @Override
        public Number[] convert(ParsedValue<ParsedValue[], Number[]> value, Font font) {
            ParsedValue[] sizes = value.getValue();
            Number[] doubles = new Number[sizes.length];
            for (int i = 0; i < sizes.length; i++) {
                doubles[i] = ((Size)sizes[i].convert(font)).pixels(font);
            }
            return doubles;
        }

        @Override
        public String toString() {
            return "Size.SequenceConverter";
        }
    }

}
