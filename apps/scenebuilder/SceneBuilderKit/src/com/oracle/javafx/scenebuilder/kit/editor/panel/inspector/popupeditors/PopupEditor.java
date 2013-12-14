/*
 * Copyright (c) 2012, 2013, Oracle and/or its affiliates.
 * All rights reserved. Use is subject to license terms.
 *
 * This file is available and licensed under the following license:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the distribution.
 *  - Neither the name of Oracle Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.oracle.javafx.scenebuilder.kit.editor.panel.inspector.popupeditors;

import com.oracle.javafx.scenebuilder.kit.editor.panel.inspector.editors.EditorUtils;
import com.oracle.javafx.scenebuilder.kit.editor.panel.inspector.editors.PropertyEditor;
import com.oracle.javafx.scenebuilder.kit.metadata.property.ValuePropertyMetadata;
import java.util.Set;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.control.MenuButton;
import javafx.scene.layout.Pane;

/**
 * Abstract class for all popup editors (font, paint, ...).
 *
 */
public abstract class PopupEditor extends PropertyEditor implements PopupEditorValidation {

    @FXML
    MenuButton popupMb;

    @FXML
    Pane editorHost;

    private PopupEditor editor;
    private Object value;

    @SuppressWarnings("LeakingThisInConstructor")
    public PopupEditor(ValuePropertyMetadata propMeta, Set<Class<?>> selectedClasses) {
        super(propMeta, selectedClasses);
        EditorUtils.loadPopupFxml("PopupEditor.fxml", this);
    }

    // Plug the concrete popup editor to the menu button.
    public void plugEditor(PopupEditor editor, Node editorContent) {
        this.editor = editor;
        editorHost.getChildren().add(editorContent);
    }

    @Override
    public Node getValueEditor() {
        return super.handleGenericModes(popupMb);
    }

    @Override
    public Object getValue() {
        return value;
    }

    @Override
    public void setValue(Object value) {
//        System.out.println(getPropertyNameText() + " - setValue() : " + value);
        setValueGeneric(value);
        if (isSetValueDone()) {
            return;
        }

        editor.setPopupContentValue(value);
    }

    @Override
    public void reset(ValuePropertyMetadata propMeta, Set<Class<?>> selectedClasses) {
//        System.out.println(getPropertyNameText() + " : resetPopupContent()");
        super.reset(propMeta, selectedClasses);

        popupMb.setText(null);
        editor.resetPopupContent();
    }

    @Override
    protected void valueIsIndeterminate() {
        handleIndeterminate(popupMb);
    }

    @Override
    public void requestFocus() {
        EditorUtils.doNextFrame(new Runnable() {

            @Override
            public void run() {
                popupMb.requestFocus();
            }
        });
    }

    /*
     * PopupEditorValidation interface.
     * Methods to be used by concrete popup editors
     */
    @Override
    public void commitValue(Object value, String displayString) {
        userUpdateValueProperty(value);
        if (displayString != null) {
            popupMb.setText(displayString);
        }
    }

    @Override
    public void transientValue(Object value) {
        // Requires support on the model side.
        // In the meantime, commit it
        commitValue(value, null);
    }

    @Override
    public void displayValueAsString(String strValue) {
        popupMb.setText(strValue);
    }

    @Override
    public void invalidValue(Object value) {
        // TBD
    }

    /*
     * Methods to be implemented by concrete popup editors.
     */
    public abstract void setPopupContentValue(Object value);

    public abstract void resetPopupContent();

}
