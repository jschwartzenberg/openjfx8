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

package javafx.animation;

/**
Builder class for javafx.animation.PauseTransition
@see javafx.animation.PauseTransition
@deprecated This class is deprecated and will be removed in the next version
* @since JavaFX 2.0
*/
@javax.annotation.Generated("Generated by javafx.builder.processor.BuilderProcessor")
@Deprecated
public final class PauseTransitionBuilder extends javafx.animation.TransitionBuilder<javafx.animation.PauseTransitionBuilder> implements javafx.util.Builder<javafx.animation.PauseTransition> {
    protected PauseTransitionBuilder() {
    }

    /** Creates a new instance of PauseTransitionBuilder. */
    @SuppressWarnings({"deprecation", "rawtypes", "unchecked"})
    public static javafx.animation.PauseTransitionBuilder create() {
        return new javafx.animation.PauseTransitionBuilder();
    }

    private boolean __set;
    public void applyTo(javafx.animation.PauseTransition x) {
        super.applyTo(x);
        if (__set) x.setDuration(this.duration);
    }

    private javafx.util.Duration duration;
    /**
    Set the value of the {@link javafx.animation.PauseTransition#getDuration() duration} property for the instance constructed by this builder.
    */
    public javafx.animation.PauseTransitionBuilder duration(javafx.util.Duration x) {
        this.duration = x;
        __set = true;
        return this;
    }

    /**
    Make an instance of {@link javafx.animation.PauseTransition} based on the properties set on this builder.
    */
    public javafx.animation.PauseTransition build() {
        javafx.animation.PauseTransition x = new javafx.animation.PauseTransition();
        applyTo(x);
        return x;
    }
}
