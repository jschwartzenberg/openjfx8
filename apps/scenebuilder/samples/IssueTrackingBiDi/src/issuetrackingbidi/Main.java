/*
 * Copyright (c) 2012, 2014, Oracle and/or its affiliates.
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
package issuetrackingbidi;

import java.util.Locale;
import java.util.ResourceBundle;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.geometry.NodeOrientation;
import javafx.scene.Scene;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;

public class Main extends Application {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Application.launch(Main.class, (java.lang.String[]) null);
    }

    @Override
    public void start(Stage primaryStage) {
        try {
            // Use one of the following to change the Language
            //  ar: Arabic  (will resolve I18N keys against Bundle.properties_ar)
            //  en: English (will resolve I18N keys against Bundle.properties)
            //  fr: French  (will resolve I18N keys against Bundle.properties_fr)
            //  iw: Hebrew  (will resolve I18N keys against Bundle.properties_iw)
            String language = "ar"; // <-- EDIT THIS

            Locale fakeLocale = new Locale(language);
            ResourceBundle i18nBundle = ResourceBundle.getBundle("issuetrackingbidi.Bundle", fakeLocale);

            AnchorPane page = (AnchorPane) FXMLLoader.load(Main.class.getResource("IssueTrackingBiDi.fxml"), i18nBundle);
            Scene scene = new Scene(page);
            primaryStage.setScene(scene);

            if ("ar".equals(language) || "iw".equals(language)) {
                scene.setNodeOrientation(NodeOrientation.RIGHT_TO_LEFT);
            }

            primaryStage.setTitle(i18nBundle.getString("application.title"));
            primaryStage.show();
        } catch (Exception ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}
