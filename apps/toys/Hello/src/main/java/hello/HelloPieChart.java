/*
 * Copyright (c) 2010, 2013, Oracle and/or its affiliates. All rights reserved.
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

package hello;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.Scene;
import javafx.scene.chart.PieChart;
import javafx.stage.Stage;

public class HelloPieChart extends Application {

    @Override public void start(Stage stage) {
        stage.setTitle("Hello PieChart");
        final PieChart pc = new PieChart();
         // setup chart
        pc.setTitle("Pie Chart Example");
        // add starting data
        ObservableList<PieChart.Data> data = FXCollections.observableArrayList();
//        for (int i=0; i<5; i++) data.add(new PieChart.Data("pie"+i, Math.random()*100));
        data.add(new PieChart.Data("Sun", 20));
         data.add(new PieChart.Data("IBM", 12));
         data.add(new PieChart.Data("HP", 25));
         data.add(new PieChart.Data("Dell", 22));
         data.add(new PieChart.Data("Apple", 30));
        pc.getData().addAll(data);


        Scene scene = new Scene(pc, 500, 500);
        stage.setScene(scene);
        stage.show();
    }


    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Application.launch(args);
    }
}
