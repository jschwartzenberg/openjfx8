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

package javafx.scene.paint;

/**
Builder class for javafx.scene.paint.ImagePattern
@see javafx.scene.paint.ImagePattern
@deprecated This class is deprecated and will be removed in the next version
*/
@javax.annotation.Generated("Generated by javafx.builder.processor.BuilderProcessor")
@Deprecated
public final class ImagePatternBuilder implements javafx.util.Builder<javafx.scene.paint.ImagePattern> {
    protected ImagePatternBuilder() {
    }
    
    /** Creates a new instance of ImagePatternBuilder. */
    @SuppressWarnings({"deprecation", "rawtypes", "unchecked"})
    public static javafx.scene.paint.ImagePatternBuilder create() {
        return new javafx.scene.paint.ImagePatternBuilder();
    }
    
    private double height;
    /**
    Set the value of the {@link javafx.scene.paint.ImagePattern#getHeight() height} property for the instance constructed by this builder.
    */
    public javafx.scene.paint.ImagePatternBuilder height(double x) {
        this.height = x;
        return this;
    }
    
    private javafx.scene.image.Image image;
    /**
    Set the value of the {@link javafx.scene.paint.ImagePattern#getImage() image} property for the instance constructed by this builder.
    */
    public javafx.scene.paint.ImagePatternBuilder image(javafx.scene.image.Image x) {
        this.image = x;
        return this;
    }
    
    private boolean proportional;
    /**
    Set the value of the {@link javafx.scene.paint.ImagePattern#isProportional() proportional} property for the instance constructed by this builder.
    */
    public javafx.scene.paint.ImagePatternBuilder proportional(boolean x) {
        this.proportional = x;
        return this;
    }
    
    private double width;
    /**
    Set the value of the {@link javafx.scene.paint.ImagePattern#getWidth() width} property for the instance constructed by this builder.
    */
    public javafx.scene.paint.ImagePatternBuilder width(double x) {
        this.width = x;
        return this;
    }
    
    private double x;
    /**
    Set the value of the {@link javafx.scene.paint.ImagePattern#getX() x} property for the instance constructed by this builder.
    */
    public javafx.scene.paint.ImagePatternBuilder x(double x) {
        this.x = x;
        return this;
    }
    
    private double y;
    /**
    Set the value of the {@link javafx.scene.paint.ImagePattern#getY() y} property for the instance constructed by this builder.
    */
    public javafx.scene.paint.ImagePatternBuilder y(double x) {
        this.y = x;
        return this;
    }
    
    /**
    Make an instance of {@link javafx.scene.paint.ImagePattern} based on the properties set on this builder.
    */
    public javafx.scene.paint.ImagePattern build() {
        javafx.scene.paint.ImagePattern x = new javafx.scene.paint.ImagePattern(this.image, this.x, this.y, this.width, this.height, this.proportional);
        return x;
    }
}
