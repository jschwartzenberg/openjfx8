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

package javafx.beans.property;

import javafx.beans.value.ObservableValue;
import javafx.beans.value.WritableValue;

/**
 * Generic interface that defines the methods common to all (writable)
 * properties independent of their type.
 * 
 * 
 * @param <T>
 *            the type of the wrapped value
 * @since JavaFX 2.0
 */
public interface Property<T> extends ReadOnlyProperty<T>, WritableValue<T> {

    /**
     * Create a unidirection binding for this {@code Property}.
     * 
     * @param observable
     *            The observable this {@code Property} should be bound to.
     * @throws NullPointerException
     *             if {@code observable} is {@code null}
     */
    void bind(ObservableValue<? extends T> observable);

    /**
     * Remove the unidirectional binding for this {@code Property}.
     * 
     * If the {@code Property} is not bound, calling this method has no effect.
     */
    void unbind();

    /**
     * Can be used to check, if a {@code Property} is bound.
     * 
     * @return {@code true} if the {@code Property} is bound, {@code false}
     *         otherwise
     */
    boolean isBound();

    /**
     * Create a bidirectional binding between this {@code Property} and another
     * one.
     * 
     * @param other
     *            the other {@code Property}
     * @throws NullPointerException
     *             if {@code other} is {@code null}
     * @throws IllegalArgumentException
     *             if {@code other} is {@code this}
     */
    void bindBidirectional(Property<T> other);

    /**
     * Remove a bidirectional binding between this {@code Property} and another
     * one.
     * 
     * If no bidirectional binding between the properties exists, calling this
     * method has no effect.
     * 
     * @param other
     *            the other {@code Property}
     * @throws NullPointerException
     *             if {@code other} is {@code null}
     * @throws IllegalArgumentException
     *             if {@code other} is {@code this}
     */
    void unbindBidirectional(Property<T> other);

}
