/*
 * Copyright (c) 2014, Oracle and/or its affiliates.
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
package ensemble.samples.controls.dialog;

import java.io.PrintWriter;
import java.io.StringWriter;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Button;
import javafx.scene.control.ButtonType;
import javafx.scene.control.Dialog;
import javafx.scene.control.DialogPane;
import javafx.scene.control.Label;
import javafx.scene.control.MenuItem;
import javafx.scene.control.SplitMenuButton;
import javafx.scene.control.Tooltip;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextInputDialog;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;
import com.sun.javafx.scene.control.skin.AccordionSkin;

/**
 * A sample that demonstrates the Dialog control.
 *
 * @sampleName DialogApp
 * @see javafx.scene.control.Dialog
 * @related /Controls/Dialog
 */
public class DialogApp extends Application {
    
    /**
     * Java main for when running without JavaFX launcher
     * @param args command line arguments
     */
    public static void main(String[] args) {
        Application.launch(args);
    }
    
    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setScene(new Scene(createContent()));
        primaryStage.show();
        
        stage = primaryStage;
    }
    
    private AlertType type = AlertType.INFORMATION;
    private Stage     stage;
    
    public void setAlertType(AlertType at) {
        type = at;
    }

    protected Alert createAlert() {
        Alert alert = new Alert(type, "");
        alert.initModality(Modality.NONE);
        alert.initOwner(stage);
        alert.getDialogPane().setContentText(type + " text.");
        alert.getDialogPane().setHeaderText(null);
        alert.show();
        
        return alert;
    }
    
    protected Dialog<ButtonType> createExceptionDialog(Throwable th) {
        Dialog<ButtonType> dialog = new Dialog<ButtonType>();
        
        dialog.setTitle("Program exception");
        
        final DialogPane dialogPane = dialog.getDialogPane();
        dialogPane.setContentText("Details of the problem:");
        dialogPane.getButtonTypes().addAll(ButtonType.OK);
        dialogPane.setContentText(th.getMessage());

        Label label = new Label("Exception stacktrace:");
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        th.printStackTrace(pw);
        pw.close();
        
        TextArea textArea = new TextArea(sw.toString());
        textArea.setEditable(false);
        textArea.setWrapText(true);
        
        textArea.setMaxWidth(Double.MAX_VALUE);
        textArea.setMaxHeight(Double.MAX_VALUE);
        GridPane.setVgrow(textArea, Priority.ALWAYS);
        GridPane.setHgrow(textArea, Priority.ALWAYS);
        
        GridPane root = new GridPane();
        root.setVisible(false);
        root.setMaxWidth(Double.MAX_VALUE);
        root.add(label, 0, 0);
        root.add(textArea, 0, 1);
        dialogPane.setExpandableContent(root);

        dialog.show();
       
        return dialog;
    }
    
    protected Dialog createTextInputDialog() {
        TextInputDialog textInput = new TextInputDialog("");
        textInput.setTitle("Text Input Dialog");
        textInput.getDialogPane().setContentText("First Name:");
        textInput.show();
        return textInput;
    }

    public Parent createContent() {
        Group group = new Group();

        HBox alertBox = new HBox(10);
        VBox dialogCreators = new VBox(20);
        
        MenuItem information = new MenuItem("Information");
        MenuItem warning = new MenuItem("Warning");
        MenuItem confirmation = new MenuItem("Confirmation");
        MenuItem error = new MenuItem("Error");

        SplitMenuButton alerts = new SplitMenuButton();
        alerts.setTooltip(new Tooltip("Choose an Alert type"));
        alerts.setOnAction(e -> System.out.println("alerts"));
        alerts.getItems().addAll(information, warning, confirmation, error);
        alerts.setText(alerts.getItems().get(0).getText());
        alerts.setOnAction(alerts.getItems().get(0).getOnAction());

        information.setOnAction(e -> {
            alerts.setText(information.getText());
            alerts.setOnAction(information.getOnAction());
            setAlertType(AlertType.INFORMATION);
        });
        warning.setOnAction(e -> {
            alerts.setText(warning.getText());
            alerts.setOnAction(warning.getOnAction());
            setAlertType(AlertType.WARNING);
        });
        confirmation.setOnAction(e -> {
            alerts.setText(confirmation.getText());
            alerts.setOnAction(confirmation.getOnAction());
            setAlertType(AlertType.CONFIRMATION);
        });
        error.setOnAction(e -> {
            alerts.setText(error.getText());
            alerts.setOnAction(error.getOnAction());
            setAlertType(AlertType.ERROR);
        });

        Button create = new Button("Create Alert");
        create.setTooltip(new Tooltip("Create an Alert Dialog"));
        create.setOnAction(e -> createAlert());
        
        alertBox.getChildren().addAll(alerts, create);
        
        Button exception = new Button("Create Exception Dialog");
        exception.setTooltip(new Tooltip("Create an Exception Dialog"));
        exception.setOnAction(e -> createExceptionDialog(new RuntimeException("oops")));
        
        Button input = new Button("Create Text Input Dialog");
        input.setTooltip(new Tooltip("Create an Text Input Dialog"));
        input.setOnAction(e -> createTextInputDialog());

        dialogCreators.getChildren().addAll(alertBox, exception, input);
        
        group.getChildren().add(dialogCreators);

        return group;
    }
}