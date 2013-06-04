/*
 * Copyright (c) 2010, 2013 Oracle and/or its affiliates.
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
package com.javafx.experiments.jfx3dviewer;

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLDecoder;
import java.util.Arrays;
import java.util.ResourceBundle;
import javafx.animation.Interpolator;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.geometry.Bounds;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.control.Accordion;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.CheckMenuItem;
import javafx.scene.control.Label;
import javafx.scene.control.SplitMenuButton;
import javafx.scene.control.SplitPane;
import javafx.scene.control.ToggleButton;
import javafx.scene.input.DragEvent;
import javafx.scene.input.Dragboard;
import javafx.scene.input.TransferMode;
import javafx.scene.layout.Region;
import javafx.scene.shape.Box;
import javafx.scene.shape.MeshView;
import javafx.scene.shape.TriangleMesh;
import javafx.stage.FileChooser;
import javafx.util.Duration;
import javafx.util.Pair;
import com.javafx.experiments.exporters.fxml.FXMLExporter;
import com.javafx.experiments.exporters.javasource.JavaSourceExporter;
import com.javafx.experiments.importers.Importer3D;
import com.javafx.experiments.importers.Optimizer;

/**
 * Controller class for main fxml file.
 */
public class MainController implements Initializable {
    public SplitMenuButton openMenuBtn;
    public Label status;
    public SplitPane splitPane;
    public ToggleButton settingsBtn;
    public CheckMenuItem loadAsPolygonsCheckBox;
    public CheckMenuItem optimizeCheckBox;
    private Accordion settingsPanel;
    private double settingsLastWidth = -1;
    private int nodeCount = 0;
    private int meshCount = 0;
    private int triangleCount = 0;
    private final ContentModel contentModel = Jfx3dViewerApp.getContentModel();
    private File loadedPath = null;
    private String[] supportedFormatRegex;

    @Override public void initialize(URL location, ResourceBundle resources) {
        try {
            // CREATE NAVIGATOR CONTROLS
            Parent navigationPanel = FXMLLoader.load(MainController.class.getResource("navigation.fxml"));
            // CREATE SETTINGS PANEL
            settingsPanel = FXMLLoader.load(MainController.class.getResource("settings.fxml"));
            // SETUP SPLIT PANE
            splitPane.getItems().addAll(new SubSceneResizer(contentModel.getSubScene(),navigationPanel), settingsPanel);
            splitPane.getDividers().get(0).setPosition(1);
        } catch (IOException e) {
            e.printStackTrace();
        }
        // listen for drops
        supportedFormatRegex = Importer3D.getSupportedFormatExtensionFilters();
        for (int i=0; i< supportedFormatRegex.length; i++) {
            supportedFormatRegex[i] = "."+supportedFormatRegex[i].replaceAll("\\.","\\.");
            System.out.println("supportedFormatRegex[i] = " + supportedFormatRegex[i]);
        }
        contentModel.getSubScene().setOnDragOver(
                new EventHandler<DragEvent>() {
                    @Override public void handle(DragEvent event) {
                        Dragboard db = event.getDragboard();
                        if (db.hasFiles()) {
                            boolean hasSupportedFile = false;
                            fileLoop: for (File file : db.getFiles()) {
                                for (String format : supportedFormatRegex) {
                                    if (file.getName().matches(format)) {
                                        hasSupportedFile = true;
                                        break fileLoop;
                                    }
                                }
                            }
                            if (hasSupportedFile) event.acceptTransferModes(TransferMode.COPY_OR_MOVE);
                        }
                        event.consume();
                    }
                });
        contentModel.getSubScene().setOnDragDropped(
                new EventHandler<DragEvent>() {
                    @Override public void handle(DragEvent event) {
                        System.out.println("DROP event = " + event);
                        Dragboard db = event.getDragboard();
                        boolean success = false;
                        if (db.hasFiles()) {
                            System.out.println("Dropped: " + db.getFiles());
                            File supportedFile = null;
                            fileLoop: for (File file : db.getFiles()) {
                                for (String format : supportedFormatRegex) {
                                    if (file.getName().matches(format)) {
                                        supportedFile = file;
                                        break fileLoop;
                                    }
                                }
                            }
                            if (supportedFile!=null) {
                                // workaround for RT-30195
                                if (supportedFile.getAbsolutePath().indexOf('%') != -1) {
                                    supportedFile = new File(URLDecoder.decode(supportedFile.getAbsolutePath()));
                                }
                                load(supportedFile);
                            }
                            success = true;
                        }
                        event.setDropCompleted(success);
                        event.consume();
                    }
                });
        // do initial status update
        updateStatus();
    }

    public void open(ActionEvent actionEvent) {
        FileChooser chooser = new FileChooser();
        chooser.getExtensionFilters().add(new FileChooser.ExtensionFilter("Supported files", Importer3D.getSupportedFormatExtensionFilters()));
        if (loadedPath != null) {
            chooser.setInitialDirectory(loadedPath.getAbsoluteFile().getParentFile());
        }
        chooser.setTitle("Select file to load");
        File newFile = chooser.showOpenDialog(openMenuBtn.getScene().getWindow());
        if (newFile != null) {
            load(newFile);
        }
    }

    private void load(File file) {
        loadedPath = file;
        try {
            Pair<Node,Timeline> content = Importer3D.loadIncludingAnimation(
                    file.toURI().toURL().toString(),
                    loadAsPolygonsCheckBox.isSelected());
            if (optimizeCheckBox.isSelected()) {
                new Optimizer(content.getValue(),content.getKey()).optimize();
            }
            contentModel.set3dContent(content.getKey());
            contentModel.setTimeline(content.getValue());
        } catch (IOException e) {
            e.printStackTrace();
        }
        updateStatus();
    }

    private void updateStatus() {
        nodeCount = 0;
        meshCount = 0;
        triangleCount = 0;
        updateCount(contentModel.getRoot3D());
        final Bounds bounds = contentModel.get3dContent().getBoundsInLocal();
        status.setText(
                String.format("Nodes [%d] :: Meshes [%d] :: Triangles [%d] :: " +
                               "Bounds [w=%.2f,h=%.2f,d=%.2f]",
                              nodeCount,meshCount,triangleCount,
                              bounds.getWidth(),bounds.getHeight(),bounds.getDepth()));
    }

    private void updateCount(Node node){
        nodeCount ++;
        if (node instanceof Parent) {
            for(Node child: ((Parent)node).getChildrenUnmodifiable()) {
                updateCount(child);
            }
        } else if (node instanceof Box) {
            meshCount ++;
            triangleCount += 6*2;
        } else if (node instanceof MeshView) {
            TriangleMesh mesh = (TriangleMesh)((MeshView)node).getMesh();
            if (mesh != null) {
                meshCount ++;
                triangleCount += mesh.getFaces().size() / TriangleMesh.NUM_COMPONENTS_PER_FACE;
            }
        }
    }

    public void toggleSettings(ActionEvent event) {
        final SplitPane.Divider divider = splitPane.getDividers().get(0);
        if (settingsBtn.isSelected()) {
            if (settingsLastWidth == -1) {
                settingsLastWidth = settingsPanel.prefWidth(-1);
            }
            final double divPos = 1 - (settingsLastWidth / splitPane.getWidth());
            new Timeline(
                    new KeyFrame(Duration.seconds(0.3),
                                 new EventHandler<ActionEvent>() {
                                     @Override public void handle(ActionEvent event) {
                                         settingsPanel.setMinWidth(Region.USE_PREF_SIZE);
                                     }
                                 },
                                 new KeyValue(divider.positionProperty(),divPos, Interpolator.EASE_BOTH)
                    )
            ).play();
        } else {
            settingsLastWidth = settingsPanel.getWidth();
            settingsPanel.setMinWidth(0);
            new Timeline(new KeyFrame(Duration.seconds(0.3),new KeyValue(divider.positionProperty(),1))).play();
        }
    }

    public void export(ActionEvent event) {
        FileChooser chooser = new FileChooser();
        if (loadedPath != null) {
            chooser.setInitialDirectory(loadedPath.getAbsoluteFile().getParentFile());
        }
        chooser.getExtensionFilters().addAll(
                new FileChooser.ExtensionFilter("FXML","*.fxml"),
                new FileChooser.ExtensionFilter("Java Source","*.java")
        );
        chooser.setTitle("Export 3D Model");
        File newFile = chooser.showSaveDialog(openMenuBtn.getScene().getWindow());
        if (newFile != null) {
            String extension = newFile.getName().substring(newFile.getName().lastIndexOf('.')+1,newFile.getName().length()).toLowerCase();
            System.out.println("extension = " + extension);
            if ("java".equals(extension)) {
                final String url = contentModel.getLoadedUrl();
                final String baseUrl = url.substring(0, url.lastIndexOf('/'));
                // try and work out package name from file
                StringBuilder packageName = new StringBuilder();
                String[] pathSegments = newFile.getAbsolutePath().split(File.separatorChar == '\\'?"\\\\":"/");
                System.out.println("pathSegments = " + Arrays.toString(pathSegments));
                loop: for (int i = pathSegments.length-2; i >= 0; i -- ) {
                    switch (pathSegments[i]){
                        case "com":
                        case "org":
                        case "net":
                            packageName.insert(0,pathSegments[i]);
                            break loop;
                        case "src":
                            packageName.deleteCharAt(0);
                            break loop;
                        case "main":
                            if ("src".equals(pathSegments[i-1])) {
                                packageName.deleteCharAt(0);
                                break loop;
                            }
                        default:
                            packageName.insert(0,'.'+pathSegments[i]);
                            break;
                    }
                    // if we get all way to root of file system then we have failed
                    if (i==0) packageName = null;
                }
                System.out.println(" packageName = " + packageName);

                JavaSourceExporter javaSourceExporter = new JavaSourceExporter(
                        baseUrl,
                        contentModel.get3dContent(),
                        contentModel.getTimeline(),
                        packageName==null?null:packageName.toString(),
                        newFile);
                javaSourceExporter.export();
            } else if ("fxml".equals(extension)) {
                new FXMLExporter(newFile.getAbsolutePath()).export(contentModel.get3dContent());
            } else {
                System.err.println("Can not export a file of type [."+extension+"]");
            }
        }
    }
}
