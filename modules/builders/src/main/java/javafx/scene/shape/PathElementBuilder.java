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

package javafx.scene.shape;

/**
Builder class for javafx.scene.shape.PathElement
@see javafx.scene.shape.PathElement
@deprecated This class is deprecated and will be removed in the next version
* @since JavaFX 2.0
*/
@javax.annotation.Generated("Generated by javafx.builder.processor.BuilderProcessor")
@Deprecated
public abstract class PathElementBuilder<B extends javafx.scene.shape.PathElementBuilder<B>> {
    protected PathElementBuilder() {
    }
    
    
    private boolean __set;
    public void applyTo(javafx.scene.shape.PathElement x) {
        if (__set) x.setAbsolute(this.absolute);
    }
    
    private boolean absolute;
    /**
    Set the value of the {@link javafx.scene.shape.PathElement#isAbsolute() absolute} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B absolute(boolean x) {
        this.absolute = x;
        __set = true;
        return (B) this;
    }
    
}
