/*
 * Copyright (c) 2010, 2011, Oracle and/or its affiliates. All rights reserved.
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

package javafx.scene.control;

import javafx.beans.property.BooleanProperty;
import javafx.beans.property.BooleanPropertyBase;
import javafx.beans.property.SimpleBooleanProperty;

import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import com.sun.javafx.css.StyleManager;

/**
 * A tri-state selection Control typically skinned as a box with a checkmark or
 * tick mark when checked. A CheckBox control can be in one of three states:
 * <ul>
 *  <li><em>checked</em>: indeterminate == false, checked == true</li>
 *  <li><em>unchecked</em>: indeterminate == false, checked == false</li>
 *  <li><em>undefined</em>: indeterminate == true</li>
 * </ul>
 * If a CheckBox is checked, then it is also by definition defined. When
 * checked the CheckBox is typically rendered with a "check" or "tick" mark.
 * A CheckBox is in this state if selected is true and indeterminate is false.
 * <p>
 * A CheckBox is unchecked if selected is false and indeterminate is false.
 * <p>
 * A CheckBox is undefined if indeterminate is true, regardless of the state
 * of selected. A typical rendering would be with a minus or dash, to
 * indicate an undefined or indeterminate state of the CheckBox.
 * This is convenient for constructing tri-state checkbox
 * based trees, for example, where undefined check boxes typically mean "inherit
 * settings from the parent".
 * <p>
 * The allowIndeterminate variable, if true, allows the user
 * to cycle through the undefined state. If false, the CheckBox is
 * not in the indeterminate state, and the user is allowed to change only the checked
 * state.
 *
 * <p>Example:
 * <pre><code>CheckBox cb = new CheckBox("a checkbox");
 * cb.setIndeterminate(false);</code></pre>
 *
 * <p>
 * MnemonicParsing is enabled by default for CheckBox.
 * </p>
 *
 */

public class CheckBox extends ButtonBase {

    /***************************************************************************
     *                                                                         *
     * Constructors                                                            *
     *                                                                         *
     **************************************************************************/

    /**
     * Creates a check box with an empty string for its label.
     */
    public CheckBox() {
        initialize();
    }

    /**
     * Creates a check box with the specified text as its label.
     *
     * @param text A text string for its label.
     */
    public CheckBox(String text) {
        setText(text);
        initialize();
    }

    private void initialize() {
        getStyleClass().setAll(DEFAULT_STYLE_CLASS);
        setAlignment(Pos.CENTER_LEFT);
        setMnemonicParsing(true);     // enable mnemonic auto-parsing by default
    }
    
    /***************************************************************************
     *                                                                         *
     * Properties                                                              *
     *                                                                         *
     **************************************************************************/
    /**
     * Determines whether the CheckBox is in the indeterminate state.
     */
    private BooleanProperty indeterminate;
    public final void setIndeterminate(boolean value) {
        indeterminateProperty().set(value);
    }

    public final boolean isIndeterminate() {
        return indeterminate == null ? false : indeterminate.get();
    }

    public final BooleanProperty indeterminateProperty() {
        if (indeterminate == null) {
            indeterminate = new BooleanPropertyBase(false) {
                @Override protected void invalidated() {
                    impl_pseudoClassStateChanged(PSEUDO_CLASS_DETERMINATE);
                    impl_pseudoClassStateChanged(PSEUDO_CLASS_INDETERMINATE);
                    fireEvent(new ActionEvent());
                }

                @Override
                public Object getBean() {
                    return CheckBox.this;
                }

                @Override
                public String getName() {
                    return "indeterminate";
                }
            };
        }
        return indeterminate;
    }
    /**
     * Indicates whether this CheckBox is checked.
     */
    private BooleanProperty selected;
    public final void setSelected(boolean value) {
        selectedProperty().set(value);
    }

    public final boolean isSelected() {
        return selected == null ? false : selected.get();
    }

    public final BooleanProperty selectedProperty() {
        if (selected == null) {
            selected = new BooleanPropertyBase() {
                @Override protected void invalidated() {
                    impl_pseudoClassStateChanged(PSEUDO_CLASS_SELECTED);
                    fireEvent(new ActionEvent());
                }

                @Override
                public Object getBean() {
                    return CheckBox.this;
                }

                @Override
                public String getName() {
                    return "selected";
                }
            };
        }
        return selected;
    }
    /**
     * Determines whether the user toggling the CheckBox should cycle through
     * all three states: <em>checked</em>, <em>unchecked</em>, and
     * <em>undefined</em>. If {@code true} then all three states will be
     * cycled through; if {@code false} then only <em>checked</em> and
     * <em>unchecked</em> will be cycled.
     */
    private BooleanProperty allowIndeterminate;

    public final void setAllowIndeterminate(boolean value) {
        allowIndeterminateProperty().set(value);
    }

    public final boolean isAllowIndeterminate() {
        return allowIndeterminate == null ? false : allowIndeterminate.get();
    }

    public final BooleanProperty allowIndeterminateProperty() {
        if (allowIndeterminate == null) {
            allowIndeterminate =
                    new SimpleBooleanProperty(this, "allowIndeterminate");
        }
        return allowIndeterminate;
    }

    /***************************************************************************
     *                                                                         *
     * Methods                                                                 *
     *                                                                         *
     **************************************************************************/

    /**
     * Toggles the state of the {@code CheckBox}. If allowIndeterminate is
     * true, then each invocation of this function will advance the CheckBox
     * through the states checked, unchecked, and undefined. If
     * allowIndeterminate is false, then the CheckBox will only cycle through
     * the checked and unchecked states, and forcing indeterminate to equal to
     * false.
     */
    @Override public void fire() {
        if (isAllowIndeterminate()) {
            if (!isSelected() && !isIndeterminate()) {
                setIndeterminate(true);
            } else if (isSelected() && !isIndeterminate()) {
                setSelected(false);
            } else if (isIndeterminate()) {
                setSelected(true);
                setIndeterminate(false);
            }
        } else {
            setSelected(!isSelected());
            setIndeterminate(false);
        }
    }

    /***************************************************************************
     *                                                                         *
     * Stylesheet Handling                                                     *
     *                                                                         *
     **************************************************************************/

    private static final String DEFAULT_STYLE_CLASS = "check-box";
    private static final String PSEUDO_CLASS_DETERMINATE = "determinate";
    private static final String PSEUDO_CLASS_INDETERMINATE = "indeterminate";
    private static final String PSEUDO_CLASS_SELECTED = "selected";

    private static final long SELECTED_PSEUDOCLASS_STATE = StyleManager.getInstance().getPseudoclassMask("selected");
    private static final long INDETERMINATE_PSEUDOCLASS_STATE = StyleManager.getInstance().getPseudoclassMask("indeterminate");
    private static final long DETERMINATE_PSEUDOCLASS_STATE = StyleManager.getInstance().getPseudoclassMask("determinate");

    /**
     * @treatAsPrivate implementation detail
     * @deprecated This is an internal API that is not intended for use and will be removed in the next version
     */
    @Deprecated @Override public long impl_getPseudoClassState() {
        long mask = super.impl_getPseudoClassState();
        if (isSelected()) mask |= SELECTED_PSEUDOCLASS_STATE;
        mask |= isIndeterminate() ? INDETERMINATE_PSEUDOCLASS_STATE : DETERMINATE_PSEUDOCLASS_STATE;
        return mask;
}

}
