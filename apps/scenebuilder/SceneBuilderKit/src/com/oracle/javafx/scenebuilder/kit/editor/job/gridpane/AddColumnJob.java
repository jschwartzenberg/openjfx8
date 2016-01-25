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
package com.oracle.javafx.scenebuilder.kit.editor.job.gridpane;

import com.oracle.javafx.scenebuilder.kit.editor.EditorController;
import com.oracle.javafx.scenebuilder.kit.editor.job.BatchSelectionJob;
import com.oracle.javafx.scenebuilder.kit.editor.job.Job;
import com.oracle.javafx.scenebuilder.kit.editor.job.gridpane.GridPaneJobUtils.Position;
import com.oracle.javafx.scenebuilder.kit.editor.selection.AbstractSelectionGroup;
import com.oracle.javafx.scenebuilder.kit.editor.selection.GridSelectionGroup;
import com.oracle.javafx.scenebuilder.kit.editor.selection.GridSelectionGroup.Type;
import com.oracle.javafx.scenebuilder.kit.editor.selection.ObjectSelectionGroup;
import com.oracle.javafx.scenebuilder.kit.editor.selection.Selection;
import com.oracle.javafx.scenebuilder.kit.fxom.FXOMInstance;
import com.oracle.javafx.scenebuilder.kit.fxom.FXOMObject;
import com.oracle.javafx.scenebuilder.kit.metadata.util.DesignHierarchyMask;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Job invoked when adding columns.
 *
 * This job handles multi-selection as follows :
 * - if multiple GridPanes are selected, no column can be selected.
 * We add the new column to each GP, either at first position (add before)
 * or last position (add after).
 * - if multiple columns are selected, a single GridPane is selected.
 * We add new columns for each selected column, either before or after.
 *
 */
public class AddColumnJob extends BatchSelectionJob {

    // Key = target GridPane instance
    // Value = set of target column indexes for this GridPane
    private final Map<FXOMObject, Set<Integer>> targetGridPanes = new HashMap<>();
    private final Position position;

    public AddColumnJob(EditorController editorController, Position position) {
        super(editorController);
        assert position == Position.BEFORE || position == Position.AFTER;
        this.position = position;
    }

    @Override
    protected List<Job> makeSubJobs() {
        final List<Job> result = new ArrayList<>();

        if (GridPaneJobUtils.canPerformAdd(getEditorController())) {

            // Populate the target GridPane map
            assert targetGridPanes.isEmpty() == true;
            final List<FXOMObject> objectList
                    = GridPaneJobUtils.getTargetGridPanes(getEditorController());
            for (FXOMObject object : objectList) {
                final Set<Integer> indexList
                        = getTargetColumnIndexes(getEditorController(), object);
                targetGridPanes.put(object, indexList);
            }

            // Add sub jobs
            // First add the new column constraints
            final Job addConstraints = new AddColumnConstraintsJob(
                    getEditorController(), position, targetGridPanes);
            result.add(addConstraints);
            // Then move the column content
            result.addAll(moveColumnContent());
        }
        return result;
    }

    @Override
    protected String makeDescription() {
        return "Add Column " + position.name(); //NOI18N
    }

    @Override
    protected AbstractSelectionGroup getNewSelectionGroup() {
        final AbstractSelectionGroup asg;
        // Update new selection :
        // - if there is more than 1 GridPane, we select the GridPane instances
        // - if there is a single GridPane, we select the added columns
        if (targetGridPanes.size() > 1) {
            Set<FXOMObject> objects = targetGridPanes.keySet();
            asg = new ObjectSelectionGroup(objects, objects.iterator().next(), null);
        } else {
            assert targetGridPanes.size() == 1;
            final FXOMInstance targetGridPane
                    = (FXOMInstance) targetGridPanes.keySet().iterator().next();
            final Set<Integer> targetIndexes = targetGridPanes.get(targetGridPane);
            assert targetIndexes.size() >= 1;
            final Set<Integer> addedIndexes
                    = GridPaneJobUtils.getAddedIndexes(targetIndexes, position);

            asg = new GridSelectionGroup(targetGridPane, Type.COLUMN, addedIndexes);
        }
        return asg;
    }

    private List<Job> moveColumnContent() {

        final List<Job> result = new ArrayList<>();

        for (FXOMObject targetGridPane : targetGridPanes.keySet()) {

            final Set<Integer> targetIndexes = targetGridPanes.get(targetGridPane);

            final DesignHierarchyMask mask = new DesignHierarchyMask(targetGridPane);
            final int columnsSize = mask.getColumnsSize();
            final Iterator<Integer> iterator = targetIndexes.iterator();

            int shiftIndex = 0;
            int targetIndex = iterator.next();
            while (targetIndex != -1) {
                // Move the columns content :
                // - from the target index
                // - to the next target index if any or the last column index otherwise
                int fromIndex, toIndex;

                switch (position) {
                    case BEFORE:
                        // fromIndex included
                        // toIndex excluded
                        fromIndex = targetIndex;
                        if (iterator.hasNext()) {
                            targetIndex = iterator.next();
                            toIndex = targetIndex - 1;
                        } else {
                            targetIndex = -1;
                            toIndex = columnsSize - 1;
                        }
                        break;
                    case AFTER:
                        // fromIndex excluded
                        // toIndex included
                        fromIndex = targetIndex + 1;
                        if (iterator.hasNext()) {
                            targetIndex = iterator.next();
                            toIndex = targetIndex;
                        } else {
                            targetIndex = -1;
                            toIndex = columnsSize - 1;
                        }
                        break;
                    default:
                        assert false;
                        return result;
                }

                // If fromIndex >= columnsSize, we are below the last existing column
                // => no column content to move
                if (fromIndex < columnsSize) {
                    final int offset = 1 + shiftIndex;
                    final List<Integer> indexes
                            = GridPaneJobUtils.getIndexes(fromIndex, toIndex);
                    final ReIndexColumnContentJob reIndexJob = new ReIndexColumnContentJob(
                            getEditorController(), offset, targetGridPane, indexes);
                    result.add(reIndexJob);
                }

                shiftIndex++;
            }
        }
        return result;
    }

    /**
     * Returns the list of target column indexes for the specified GridPane
     * instance.
     *
     * @return the list of target indexes
     */
    private Set<Integer> getTargetColumnIndexes(
            final EditorController editorController,
            final FXOMObject targetGridPane) {

        final Selection selection = editorController.getSelection();
        final AbstractSelectionGroup asg = selection.getGroup();

        final Set<Integer> result = new LinkedHashSet<>();

        // Selection == GridPane columns
        // => return the list of selected columns
        if (asg instanceof GridSelectionGroup
                && ((GridSelectionGroup) asg).getType() == Type.COLUMN) {
            final GridSelectionGroup gsg = (GridSelectionGroup) asg;
            result.addAll(gsg.getIndexes());
        } //
        // Selection == GridPanes or Selection == GridPane rows
        // => return either the first (BEFORE) or the last (AFTER) column index
        else {
            switch (position) {
                case BEFORE:
                    result.add(0);
                    break;
                case AFTER:
                    final DesignHierarchyMask mask
                            = new DesignHierarchyMask(targetGridPane);
                    final int size = mask.getColumnsSize();
                    result.add(size - 1);
                    break;
                default:
                    assert false;
                    break;
            }
        }

        return result;
    }
}
