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

package javafx.scene.control;

import com.sun.javafx.runtime.VersionInfo;
import com.sun.javafx.scene.control.test.ControlAsserts;
import com.sun.javafx.scene.control.test.Employee;
import com.sun.javafx.scene.control.test.Person;
import com.sun.javafx.scene.control.test.RT_22463_Person;
import com.sun.javafx.tk.Toolkit;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import javafx.beans.InvalidationListener;
import javafx.beans.Observable;
import static javafx.scene.control.ControlTestUtils.assertStyleClassContains;
import static org.junit.Assert.*;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import javafx.util.Callback;

import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

public class TreeViewTest {
    private TreeView<String> treeView;
    
    // sample data #1
    private TreeItem<String> root;
    private TreeItem<String> child1;
    private TreeItem<String> child2;
    private TreeItem<String> child3;
    
    // sample data #1
    private TreeItem<String> myCompanyRootNode;
        private TreeItem<String> salesDepartment;
            private TreeItem<String> ethanWilliams;
            private TreeItem<String> emmaJones;
            private TreeItem<String> michaelBrown;
            private TreeItem<String> annaBlack;
            private TreeItem<String> rodgerYork;
            private TreeItem<String> susanCollins;

        private TreeItem<String> itSupport;
            private TreeItem<String> mikeGraham;
            private TreeItem<String> judyMayer;
            private TreeItem<String> gregorySmith;
    
    @Before public void setup() {
        treeView = new TreeView<String>();
        
        // build sample data #2, even though it may not be used...
        myCompanyRootNode = new TreeItem<String>("MyCompany Human Resources");
        salesDepartment = new TreeItem<String>("Sales Department");
            ethanWilliams = new TreeItem<String>("Ethan Williams");
            emmaJones = new TreeItem<String>("Emma Jones");
            michaelBrown = new TreeItem<String>("Michael Brown");
            annaBlack = new TreeItem<String>("Anna Black");
            rodgerYork = new TreeItem<String>("Rodger York");
            susanCollins = new TreeItem<String>("Susan Collins");

        itSupport = new TreeItem<String>("IT Support");
            mikeGraham = new TreeItem<String>("Mike Graham");
            judyMayer = new TreeItem<String>("Judy Mayer");
            gregorySmith = new TreeItem<String>("Gregory Smith");
            
        myCompanyRootNode.getChildren().setAll(
            salesDepartment,
            itSupport
        );
        salesDepartment.getChildren().setAll(
            ethanWilliams,
            emmaJones,
            michaelBrown, 
            annaBlack,
            rodgerYork,
            susanCollins
        );
        itSupport.getChildren().setAll(
            mikeGraham,
            judyMayer,
            gregorySmith
        );
    }

    private void installChildren() {
        root = new TreeItem<String>("Root");
        child1 = new TreeItem<String>("Child 1");
        child2 = new TreeItem<String>("Child 2");
        child3 = new TreeItem<String>("Child 3");
        root.setExpanded(true);
        root.getChildren().setAll(child1, child2, child3);
        treeView.setRoot(root);
    }
    
    @Test public void ensureCorrectInitialState() {
        installChildren();
        assertEquals(0, treeView.getRow(root));
        assertEquals(1, treeView.getRow(child1));
        assertEquals(2, treeView.getRow(child2));
        assertEquals(3, treeView.getRow(child3));
    }

    /*********************************************************************
     * Tests for the constructors                                        *
     ********************************************************************/
    
    @Test public void noArgConstructorSetsTheStyleClass() {
        assertStyleClassContains(treeView, "tree-view");
    }

    @Test public void noArgConstructorSetsNonNullSelectionModel() {
        assertNotNull(treeView.getSelectionModel());
    }

    @Test public void noArgConstructorSetsNullItems() {
        assertNull(treeView.getRoot());
    }

    @Test public void noArgConstructor_selectedItemIsNull() {
        assertNull(treeView.getSelectionModel().getSelectedItem());
    }

    @Test public void noArgConstructor_selectedIndexIsNegativeOne() {
        assertEquals(-1, treeView.getSelectionModel().getSelectedIndex());
    }

    @Test public void singleArgConstructorSetsTheStyleClass() {
        final TreeView<String> b2 = new TreeView<String>(new TreeItem<String>("Hi"));
        assertStyleClassContains(b2, "tree-view");
    }

    @Test public void singleArgConstructorSetsNonNullSelectionModel() {
        final TreeView<String> b2 = new TreeView<String>(new TreeItem<String>("Hi"));
        assertNotNull(b2.getSelectionModel());
    }

    @Test public void singleArgConstructorAllowsNullItems() {
        final TreeView<String> b2 = new TreeView<String>(null);
        assertNull(b2.getRoot());
    }

    @Test public void singleArgConstructor_selectedItemIsNull() {
        final TreeView<String> b2 = new TreeView<String>(new TreeItem<String>("Hi"));
        assertNull(b2.getSelectionModel().getSelectedItem());
    }

    @Test public void singleArgConstructor_selectedIndexIsNegativeOne() {
        final TreeView<String> b2 = new TreeView<String>(new TreeItem<String>("Hi"));
        assertEquals(-1, b2.getSelectionModel().getSelectedIndex());
    }

    /*********************************************************************
     * Tests for selection model                                         *
     ********************************************************************/

    @Test public void selectionModelCanBeNull() {
        treeView.setSelectionModel(null);
        assertNull(treeView.getSelectionModel());
    }

    @Test public void selectionModelCanBeBound() {
        MultipleSelectionModel<TreeItem<String>> sm = new TreeView.TreeViewBitSetSelectionModel<String>(treeView);
        ObjectProperty<MultipleSelectionModel<TreeItem<String>>> other = new SimpleObjectProperty<MultipleSelectionModel<TreeItem<String>>>(sm);
        treeView.selectionModelProperty().bind(other);
        assertSame(sm, treeView.getSelectionModel());
    }

    @Test public void selectionModelCanBeChanged() {
        MultipleSelectionModel<TreeItem<String>> sm = new TreeView.TreeViewBitSetSelectionModel<String>(treeView);
        treeView.setSelectionModel(sm);
        assertSame(sm, treeView.getSelectionModel());
    }

    @Test public void canSetSelectedItemToAnItemEvenWhenThereAreNoItems() {
        TreeItem<String> element = new TreeItem<String>("I AM A CRAZY RANDOM STRING");
        treeView.getSelectionModel().select(element);
        assertEquals(-1, treeView.getSelectionModel().getSelectedIndex());
        assertSame(element, treeView.getSelectionModel().getSelectedItem());
    }

    @Test public void canSetSelectedItemToAnItemNotInTheDataModel() {
        installChildren();
        TreeItem<String> element = new TreeItem<String>("I AM A CRAZY RANDOM STRING");
        treeView.getSelectionModel().select(element);
        assertEquals(-1, treeView.getSelectionModel().getSelectedIndex());
        assertSame(element, treeView.getSelectionModel().getSelectedItem());
    }

    @Test public void settingTheSelectedItemToAnItemInItemsResultsInTheCorrectSelectedIndex() {
        installChildren();
        treeView.getSelectionModel().select(child1);
        assertEquals(1, treeView.getSelectionModel().getSelectedIndex());
        assertSame(child1, treeView.getSelectionModel().getSelectedItem());
    }

    @Ignore("Not yet supported")
    @Test public void settingTheSelectedItemToANonexistantItemAndThenSettingItemsWhichContainsItResultsInCorrectSelectedIndex() {
        treeView.getSelectionModel().select(child1);
        installChildren();
        assertEquals(1, treeView.getSelectionModel().getSelectedIndex());
        assertSame(child1, treeView.getSelectionModel().getSelectedItem());
    }
    
    @Ignore("Not yet supported")
    @Test public void ensureSelectionClearsWhenAllItemsAreRemoved_selectIndex0() {
        installChildren();
        treeView.getSelectionModel().select(0);
        treeView.setRoot(null);
        assertEquals(-1, treeView.getSelectionModel().getSelectedIndex());
        assertEquals(null, treeView.getSelectionModel().getSelectedItem());
    }
    
    @Ignore("Not yet supported")
    @Test public void ensureSelectionClearsWhenAllItemsAreRemoved_selectIndex2() {
        installChildren();
        treeView.getSelectionModel().select(2);
        treeView.setRoot(null);
        assertEquals(-1, treeView.getSelectionModel().getSelectedIndex());
        assertEquals(null, treeView.getSelectionModel().getSelectedItem());
    }
    
    @Ignore("Not yet supported")
    @Test public void ensureSelectedItemRemainsAccurateWhenItemsAreCleared() {
        installChildren();
        treeView.getSelectionModel().select(2);
        treeView.setRoot(null);
        assertNull(treeView.getSelectionModel().getSelectedItem());
        assertEquals(-1, treeView.getSelectionModel().getSelectedIndex());
        
        TreeItem<String> newRoot = new TreeItem<String>("New Root");
        TreeItem<String> newChild1 = new TreeItem<String>("New Child 1");
        TreeItem<String> newChild2 = new TreeItem<String>("New Child 2");
        TreeItem<String> newChild3 = new TreeItem<String>("New Child 3");
        newRoot.setExpanded(true);
        newRoot.getChildren().setAll(newChild1, newChild2, newChild3);
        treeView.setRoot(root);
        
        treeView.getSelectionModel().select(2);
        assertEquals(newChild2, treeView.getSelectionModel().getSelectedItem());
    }
    
    @Test public void ensureSelectionIsCorrectWhenItemsChange() {
        installChildren();
        treeView.getSelectionModel().select(0);
        assertEquals(root, treeView.getSelectionModel().getSelectedItem());
        
        treeView.setRoot(new TreeItem<String>("New Root"));
        assertEquals(-1, treeView.getSelectionModel().getSelectedIndex());
        assertEquals(null, treeView.getSelectionModel().getSelectedItem());
    }
    
    @Test public void ensureSelectionRemainsOnBranchWhenExpanded() {
        installChildren();
        root.setExpanded(false);
        treeView.getSelectionModel().select(0);
        assertTrue(treeView.getSelectionModel().isSelected(0));
        root.setExpanded(true);
        assertTrue(treeView.getSelectionModel().isSelected(0));
        assertTrue(treeView.getSelectionModel().getSelectedItems().contains(root));
    }
    
    /*********************************************************************
     * Tests for misc                                                    *
     ********************************************************************/
    @Test public void ensureRootIndexIsZeroWhenRootIsShowing() {
        installChildren();
        assertEquals(0, treeView.getRow(root));
    }
    
    @Test public void ensureRootIndexIsNegativeOneWhenRootIsNotShowing() {
        installChildren();
        treeView.setShowRoot(false);
        assertEquals(-1, treeView.getRow(root));
    }
    
    @Test public void ensureCorrectIndexWhenRootTreeItemHasParent() {
        installChildren();
        treeView.setRoot(child1);
        assertEquals(-1, treeView.getRow(root));
        assertEquals(0, treeView.getRow(child1));
        assertEquals(1, treeView.getRow(child2));
        assertEquals(2, treeView.getRow(child3));
    }
    
    @Test public void ensureCorrectIndexWhenRootTreeItemHasParentAndRootIsNotShowing() {
        installChildren();
        treeView.setRoot(child1);
        treeView.setShowRoot(false);
        
        // despite the fact there are children in this tree, in reality none are
        // visible as the root node has no children (only siblings), and the
        // root node is not visible.
        assertEquals(0, treeView.getExpandedItemCount());
        
        assertEquals(-1, treeView.getRow(root));
        assertEquals(-1, treeView.getRow(child1));
        assertEquals(-1, treeView.getRow(child2));
        assertEquals(-1, treeView.getRow(child3));
    }
    
    @Test public void ensureCorrectIndexWhenRootTreeItemIsCollapsed() {
        installChildren();
        root.setExpanded(false);
        assertEquals(0, treeView.getRow(root));
        
        // note that the indices are still positive, representing what the values
        // would be if this row is visible
        assertEquals(1, treeView.getRow(child1));
        assertEquals(2, treeView.getRow(child2));
        assertEquals(3, treeView.getRow(child3));
    }
    
    @Test public void removingLastTest() {
        TreeView tree_view = new TreeView();
        MultipleSelectionModel sm = tree_view.getSelectionModel();
        TreeItem<String> tree_model = new TreeItem<String>("Root");
        TreeItem node = new TreeItem("Data item");
        tree_model.getChildren().add(node);
        tree_view.setRoot(tree_model);
        tree_model.setExpanded(true);
        // select the 'Data item' in the selection model
        sm.select(tree_model.getChildren().get(0));
        // remove the 'Data item' from the root node
        tree_model.getChildren().remove(sm.getSelectedItem());
        // assert the there are no selected items any longer
        assertTrue("items: " + sm.getSelectedItem(), sm.getSelectedItems().isEmpty());
    }
    
    /*********************************************************************
     * Tests from bug reports                                            *
     ********************************************************************/  
    @Ignore @Test public void test_rt17112() {
        TreeItem<String> root1 = new TreeItem<String>("Root");
        root1.setExpanded(true);
        addChildren(root1, "child");
        for (TreeItem child : root1.getChildren()) {
            addChildren(child, (String)child.getValue());
            child.setExpanded(true);
        }

        final TreeView treeView1 = new TreeView();
        final MultipleSelectionModel sm = treeView1.getSelectionModel();
        sm.setSelectionMode(SelectionMode.MULTIPLE);
        treeView1.setRoot(root1);
        
        final TreeItem<String> rt17112_child1 = root1.getChildren().get(1);
        final TreeItem<String> rt17112_child1_0 = rt17112_child1.getChildren().get(0);
        final TreeItem<String> rt17112_child2 = root1.getChildren().get(2);
        
        sm.getSelectedItems().addListener(new InvalidationListener() {
            int count = 0;
            @Override public void invalidated(Observable observable) {
                if (count == 0) {
                    assertEquals(rt17112_child1_0, sm.getSelectedItem());
                    assertEquals(1, sm.getSelectedIndices().size());
                    assertEquals(6, sm.getSelectedIndex());
                    assertTrue(treeView1.getFocusModel().isFocused(6));
                } else if (count == 1) {
                    assertEquals(rt17112_child1, sm.getSelectedItem());
                    assertFalse(sm.getSelectedItems().contains(rt17112_child2));
                    assertEquals(1, sm.getSelectedIndices().size());
                    assertTrue(treeView1.getFocusModel().isFocused(5));
                }
                count++;
            }
        });
        
        // this triggers the first callback above, so that count == 0
        sm.select(rt17112_child1_0);

        // this triggers the second callback above, so that count == 1
        rt17112_child1.setExpanded(false);
    }
    private void addChildren(TreeItem parent, String name) {
        for (int i=0; i<3; i++) {
            TreeItem<String> ti = new TreeItem<String>(name+"-"+i);
            parent.getChildren().add(ti);
        }
    }
    
    @Test public void test_rt17522_focusShouldMoveWhenItemAddedAtFocusIndex() {
        installChildren();
        FocusModel fm = treeView.getFocusModel();
        fm.focus(1);    // focus on child1
        assertTrue(fm.isFocused(1));
        assertEquals(child1, fm.getFocusedItem());
        
        TreeItem child0 = new TreeItem("child0");
        root.getChildren().add(0, child0);  // 0th index == position of child1 in root
        
        assertEquals(child1, fm.getFocusedItem());
        assertTrue(fm.isFocused(2));
    }
    
    @Test public void test_rt17522_focusShouldMoveWhenItemAddedBeforeFocusIndex() {
        installChildren();
        FocusModel fm = treeView.getFocusModel();
        fm.focus(1);    // focus on child1
        assertTrue(fm.isFocused(1));
        
        TreeItem child0 = new TreeItem("child0");
        root.getChildren().add(0, child0);
        assertTrue("Focused index: " + fm.getFocusedIndex(), fm.isFocused(2));
    }
    
    @Test public void test_rt17522_focusShouldNotMoveWhenItemAddedAfterFocusIndex() {
        installChildren();
        FocusModel fm = treeView.getFocusModel();
        fm.focus(1);    // focus on child1
        assertTrue(fm.isFocused(1));
        
        TreeItem child4 = new TreeItem("child4");
        root.getChildren().add(3, child4);
        assertTrue("Focused index: " + fm.getFocusedIndex(), fm.isFocused(1));
    }
    
    @Test public void test_rt17522_focusShouldBeResetWhenFocusedItemIsRemoved() {
        installChildren();
        FocusModel fm = treeView.getFocusModel();
        fm.focus(1);
        assertTrue(fm.isFocused(1));
        
        root.getChildren().remove(child1);
        assertEquals(-1, fm.getFocusedIndex());
        assertNull(fm.getFocusedItem());
    }
    
    @Test public void test_rt17522_focusShouldMoveWhenItemRemovedBeforeFocusIndex() {
        installChildren();
        FocusModel fm = treeView.getFocusModel();
        fm.focus(2);
        assertTrue(fm.isFocused(2));
        
        root.getChildren().remove(child1);
        assertTrue(fm.isFocused(1));
        assertEquals(child2, fm.getFocusedItem());
    }

//    This test fails as, in TreeView FocusModel, we do not know the index of the
//    removed tree items, which means we don't know whether they existed before
//    or after the focused item.
//    @Test public void test_rt17522_focusShouldNotMoveWhenItemRemovedAfterFocusIndex() {
//        installChildren();
//        FocusModel fm = treeView.getFocusModel();
//        fm.focus(1);
//        assertTrue(fm.isFocused(1));
//        
//        root.getChildren().remove(child3);
//        assertTrue("Focused index: " + fm.getFocusedIndex(), fm.isFocused(1));
//        assertEquals(child1, fm.getFocusedItem());
//    }
    
    @Test public void test_rt18385() {
        installChildren();
//        table.getItems().addAll("row1", "row2", "row3");
        treeView.getSelectionModel().select(1);
        treeView.getRoot().getChildren().add(new TreeItem("Another Row"));
        assertEquals(1, treeView.getSelectionModel().getSelectedIndices().size());
        assertEquals(1, treeView.getSelectionModel().getSelectedItems().size());
    }
    
    @Test public void test_rt18339_onlyEditWhenTreeViewIsEditable_editableIsFalse() {
        treeView.setEditable(false);
        treeView.edit(root);
        assertEquals(null, treeView.getEditingItem());
    }
    
    @Test public void test_rt18339_onlyEditWhenTreeViewIsEditable_editableIsTrue() {
        treeView.setEditable(true);
        treeView.edit(root);
        assertEquals(root, treeView.getEditingItem());
    }
    
    @Test public void test_rt14451() {
        installChildren();
        treeView.getSelectionModel().setSelectionMode(SelectionMode.MULTIPLE);
        treeView.getSelectionModel().selectRange(0, 2); // select from 0 (inclusive) to 2 (exclusive)
        assertEquals(2, treeView.getSelectionModel().getSelectedIndices().size());
    }
    
    @Test public void test_rt21586() {
        installChildren();
        treeView.getSelectionModel().select(1);
        assertEquals(1, treeView.getSelectionModel().getSelectedIndex());
        assertEquals(child1, treeView.getSelectionModel().getSelectedItem());
        
        TreeItem root = new TreeItem<String>("New Root");
        TreeItem child1 = new TreeItem<String>("New Child 1");
        TreeItem child2 = new TreeItem<String>("New Child 2");
        TreeItem child3 = new TreeItem<String>("New Child 3");
        root.setExpanded(true);
        root.getChildren().setAll(child1, child2, child3);
        treeView.setRoot(root);
        assertEquals(-1, treeView.getSelectionModel().getSelectedIndex());
        assertNull(treeView.getSelectionModel().getSelectedItem());
    }
    
    @Test public void test_rt27181() {
        myCompanyRootNode.setExpanded(true);
        treeView.setRoot(myCompanyRootNode);
        
        // start test
        salesDepartment.setExpanded(true);
        treeView.getSelectionModel().select(salesDepartment);
        
        assertEquals(1, treeView.getFocusModel().getFocusedIndex());
        itSupport.setExpanded(true);
        assertEquals(1, treeView.getFocusModel().getFocusedIndex());
    }
    
    @Test public void test_rt27185() {
        myCompanyRootNode.setExpanded(true);
        treeView.setRoot(myCompanyRootNode);
        
        // start test
        itSupport.setExpanded(true);
        treeView.getSelectionModel().select(mikeGraham);
        
        assertEquals(mikeGraham, treeView.getFocusModel().getFocusedItem());
        salesDepartment.setExpanded(true);
        assertEquals(mikeGraham, treeView.getFocusModel().getFocusedItem());
    }
    
    @Ignore("Bug hasn't been fixed yet")
    @Test public void test_rt28114() {
        myCompanyRootNode.setExpanded(true);
        treeView.setRoot(myCompanyRootNode);
        
        // start test
        itSupport.setExpanded(true);
        treeView.getSelectionModel().select(itSupport);
        assertEquals(itSupport, treeView.getFocusModel().getFocusedItem());
        assertEquals(itSupport, treeView.getSelectionModel().getSelectedItem());
        assertTrue(! itSupport.isLeaf());
        assertTrue(itSupport.isExpanded());
        
        itSupport.getChildren().remove(mikeGraham);
        assertEquals(itSupport, treeView.getFocusModel().getFocusedItem());
        assertEquals(itSupport, treeView.getSelectionModel().getSelectedItem());
        assertTrue(itSupport.isLeaf());
        assertTrue(!itSupport.isExpanded());
    }
    
    @Test public void test_rt27820_1() {
        TreeItem root = new TreeItem("root");
        root.setExpanded(true);
        TreeItem child = new TreeItem("child");
        root.getChildren().add(child);
        treeView.setRoot(root);
        
        treeView.getSelectionModel().select(0);
        assertEquals(1, treeView.getSelectionModel().getSelectedItems().size());
        assertEquals(root, treeView.getSelectionModel().getSelectedItem());
        
        treeView.setRoot(null);
        assertEquals(0, treeView.getSelectionModel().getSelectedItems().size());
        assertNull(treeView.getSelectionModel().getSelectedItem());
    }
    
    @Test public void test_rt27820_2() {
        TreeItem root = new TreeItem("root");
        root.setExpanded(true);
        TreeItem child = new TreeItem("child");
        root.getChildren().add(child);
        treeView.setRoot(root);
        
        treeView.getSelectionModel().select(1);
        assertEquals(1, treeView.getSelectionModel().getSelectedItems().size());
        assertEquals(child, treeView.getSelectionModel().getSelectedItem());
        
        treeView.setRoot(null);
        assertEquals(0, treeView.getSelectionModel().getSelectedItems().size());
        assertNull(treeView.getSelectionModel().getSelectedItem());
    }
    
    @Test public void test_rt28390() {
        // There should be no NPE when a TreeView is shown and the disclosure
        // node is null in a TreeCell
        TreeItem root = new TreeItem("root");
        treeView.setRoot(root);
        
        // install a custom cell factory that forces the disclosure node to be
        // null (because by default a null disclosure node will be replaced by
        // a non-null one).
        treeView.setCellFactory(new Callback() {
            @Override public Object call(Object p) {
                TreeCell treeCell = new TreeCell() {
                    {
                        disclosureNodeProperty().addListener(new ChangeListener() {
                            @Override public void changed(ObservableValue ov, Object t, Object t1) {
                                setDisclosureNode(null);
                            }
                        });
                    }
                    
                    @Override protected void updateItem(Object item, boolean empty) {
                        super.updateItem(item, empty);
                        setText(item == null ? "" : item.toString());
                    }
                };
                treeCell.setDisclosureNode(null);
                return treeCell;
            }
        });
        
        try {
            Group group = new Group();
            group.getChildren().setAll(treeView);
            Scene scene = new Scene(group);
            Stage stage = new Stage();
            stage.setScene(scene);
            stage.show();
        } catch (NullPointerException e) {
            System.out.println("A null disclosure node is valid, so we shouldn't have an NPE here.");
            e.printStackTrace();
            assertTrue(false);
        }
    }
    
    @Test public void test_rt28534() {
        TreeItem root = new TreeItem("root");
        root.getChildren().setAll(
                new TreeItem(new Person("Jacob", "Smith", "jacob.smith@example.com")),
                new TreeItem(new Person("Isabella", "Johnson", "isabella.johnson@example.com")),
                new TreeItem(new Person("Ethan", "Williams", "ethan.williams@example.com")),
                new TreeItem(new Person("Emma", "Jones", "emma.jones@example.com")),
                new TreeItem(new Person("Michael", "Brown", "michael.brown@example.com")));
        root.setExpanded(true);
        
        TreeView<Person> tree = new TreeView<Person>(root);
        
        ControlAsserts.assertRowsNotEmpty(tree, 0, 6); // rows 0 - 6 should be filled
        ControlAsserts.assertRowsEmpty(tree, 6, -1); // rows 6+ should be empty
        
        // now we replace the data and expect the cells that have no data
        // to be empty
        root.getChildren().setAll(
                new TreeItem(new Person("*_*Emma", "Jones", "emma.jones@example.com")),
                new TreeItem(new Person("_Michael", "Brown", "michael.brown@example.com")));
        
        ControlAsserts.assertRowsNotEmpty(tree, 0, 3); // rows 0 - 3 should be filled
        ControlAsserts.assertRowsEmpty(tree, 3, -1); // rows 3+ should be empty
    }
    
    @Test public void test_rt28556() {
        List<Employee> employees = Arrays.<Employee>asList(
            new Employee("Ethan Williams", "Sales Department"),
            new Employee("Emma Jones", "Sales Department"),
            new Employee("Michael Brown", "Sales Department"),
            new Employee("Anna Black", "Sales Department"),
            new Employee("Rodger York", "Sales Department"),
            new Employee("Susan Collins", "Sales Department"),
            new Employee("Mike Graham", "IT Support"),
            new Employee("Judy Mayer", "IT Support"),
            new Employee("Gregory Smith", "IT Support"),
            new Employee("Jacob Smith", "Accounts Department"),
            new Employee("Isabella Johnson", "Accounts Department"));
    
        TreeItem<String> rootNode = new TreeItem<String>("MyCompany Human Resources");
        rootNode.setExpanded(true);
        
        List<TreeItem<String>> nodeList = FXCollections.observableArrayList();
        for (Employee employee : employees) {
            nodeList.add(new TreeItem<String>(employee.getName()));
        }
        rootNode.getChildren().setAll(nodeList);

        TreeView<String> treeView = new TreeView<String>(rootNode);
        
        // ensure all children of the root node have the correct indentation 
        // before the sort occurs
        ControlAsserts.assertLayoutX(treeView, 1, 11, 31.0);
        for (TreeItem<String> children : rootNode.getChildren()) {
            assertEquals(rootNode, children.getParent());
        }
        
        // run sort
        Collections.sort(rootNode.getChildren(), new Comparator<TreeItem<String>>() {
            @Override public int compare(TreeItem<String> o1, TreeItem<String> o2) {
                return o1.getValue().compareTo(o2.getValue());
            }
        });
        
        // ensure the same indentation exists after the sort (which is where the
        // bug is - it drops down to 21.0px indentation when it shouldn't).
        ControlAsserts.assertLayoutX(treeView, 1, 11, 31.0);
        for (TreeItem<String> children : rootNode.getChildren()) {
            assertEquals(rootNode, children.getParent());
        }
    }
    
    @Test public void test_rt22463() {
        RT_22463_Person rootPerson = new RT_22463_Person();
        rootPerson.setName("Root");
        TreeItem<RT_22463_Person> root = new TreeItem<RT_22463_Person>(rootPerson);
        root.setExpanded(true);
        
        final TreeView<RT_22463_Person> tree = new TreeView<RT_22463_Person>();
        tree.setRoot(root);
        
        // before the change things display fine
        RT_22463_Person p1 = new RT_22463_Person();
        p1.setId(1l);
        p1.setName("name1");
        RT_22463_Person p2 = new RT_22463_Person();
        p2.setId(2l);
        p2.setName("name2");
        root.getChildren().addAll(
                new TreeItem<RT_22463_Person>(p1), 
                new TreeItem<RT_22463_Person>(p2));
        ControlAsserts.assertCellTextEquals(tree, 1, "name1");
        ControlAsserts.assertCellTextEquals(tree, 2, "name2");
        
        // now we change the persons but they are still equal as the ID's don't
        // change - but the items list is cleared so the cells should update
        RT_22463_Person new_p1 = new RT_22463_Person();
        new_p1.setId(1l);
        new_p1.setName("updated name1");
        RT_22463_Person new_p2 = new RT_22463_Person();
        new_p2.setId(2l);
        new_p2.setName("updated name2");
        root.getChildren().clear();
        root.getChildren().setAll(
                new TreeItem<RT_22463_Person>(new_p1), 
                new TreeItem<RT_22463_Person>(new_p2));
        ControlAsserts.assertCellTextEquals(tree, 1, "updated name1");
        ControlAsserts.assertCellTextEquals(tree, 2, "updated name2");
    }
}
