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

package javafx.scene;

/**
Builder class for javafx.scene.Scene
@see javafx.scene.Scene
@deprecated This class is deprecated and will be removed in the next version
* @since JavaFX 2.0
*/
@javax.annotation.Generated("Generated by javafx.builder.processor.BuilderProcessor")
@Deprecated
public class SceneBuilder<B extends javafx.scene.SceneBuilder<B>> implements javafx.util.Builder<javafx.scene.Scene> {
    protected SceneBuilder() {
    }
    
    /** Creates a new instance of SceneBuilder. */
    @SuppressWarnings({"deprecation", "rawtypes", "unchecked"})
    public static javafx.scene.SceneBuilder<?> create() {
        return new javafx.scene.SceneBuilder();
    }
    
    private long __set;
    private void __set(int i) {
        __set |= 1L << i;
    }
    public void applyTo(javafx.scene.Scene x) {
        long set = __set;
        while (set != 0) {
            int i = Long.numberOfTrailingZeros(set);
            set &= ~(1L << i);
            switch (i) {
                case 0: x.setCamera(this.camera); break;
                case 1: x.setCursor(this.cursor); break;
                case 2: x.setEventDispatcher(this.eventDispatcher); break;
                case 3: x.setFill(this.fill); break;
                case 4: x.setOnContextMenuRequested(this.onContextMenuRequested); break;
                case 5: x.setOnDragDetected(this.onDragDetected); break;
                case 6: x.setOnDragDone(this.onDragDone); break;
                case 7: x.setOnDragDropped(this.onDragDropped); break;
                case 8: x.setOnDragEntered(this.onDragEntered); break;
                case 9: x.setOnDragExited(this.onDragExited); break;
                case 10: x.setOnDragOver(this.onDragOver); break;
                case 11: x.setOnInputMethodTextChanged(this.onInputMethodTextChanged); break;
                case 12: x.setOnKeyPressed(this.onKeyPressed); break;
                case 13: x.setOnKeyReleased(this.onKeyReleased); break;
                case 14: x.setOnKeyTyped(this.onKeyTyped); break;
                case 15: x.setOnMouseClicked(this.onMouseClicked); break;
                case 16: x.setOnMouseDragEntered(this.onMouseDragEntered); break;
                case 17: x.setOnMouseDragExited(this.onMouseDragExited); break;
                case 18: x.setOnMouseDragged(this.onMouseDragged); break;
                case 19: x.setOnMouseDragOver(this.onMouseDragOver); break;
                case 20: x.setOnMouseDragReleased(this.onMouseDragReleased); break;
                case 21: x.setOnMouseEntered(this.onMouseEntered); break;
                case 22: x.setOnMouseExited(this.onMouseExited); break;
                case 23: x.setOnMouseMoved(this.onMouseMoved); break;
                case 24: x.setOnMousePressed(this.onMousePressed); break;
                case 25: x.setOnMouseReleased(this.onMouseReleased); break;
                case 26: x.setOnRotate(this.onRotate); break;
                case 27: x.setOnRotationFinished(this.onRotationFinished); break;
                case 28: x.setOnRotationStarted(this.onRotationStarted); break;
                case 29: x.setOnScroll(this.onScroll); break;
                case 30: x.setOnScrollFinished(this.onScrollFinished); break;
                case 31: x.setOnScrollStarted(this.onScrollStarted); break;
                case 32: x.setOnSwipeDown(this.onSwipeDown); break;
                case 33: x.setOnSwipeLeft(this.onSwipeLeft); break;
                case 34: x.setOnSwipeRight(this.onSwipeRight); break;
                case 35: x.setOnSwipeUp(this.onSwipeUp); break;
                case 36: x.setOnTouchMoved(this.onTouchMoved); break;
                case 37: x.setOnTouchPressed(this.onTouchPressed); break;
                case 38: x.setOnTouchReleased(this.onTouchReleased); break;
                case 39: x.setOnTouchStationary(this.onTouchStationary); break;
                case 40: x.setOnZoom(this.onZoom); break;
                case 41: x.setOnZoomFinished(this.onZoomFinished); break;
                case 42: x.setOnZoomStarted(this.onZoomStarted); break;
                case 43: x.getStylesheets().addAll(this.stylesheets); break;
            }
        }
    }
    
    private javafx.scene.Camera camera;
    /**
    Set the value of the {@link javafx.scene.Scene#getCamera() camera} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B camera(javafx.scene.Camera x) {
        this.camera = x;
        __set(0);
        return (B) this;
    }
    
    private javafx.scene.Cursor cursor;
    /**
    Set the value of the {@link javafx.scene.Scene#getCursor() cursor} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B cursor(javafx.scene.Cursor x) {
        this.cursor = x;
        __set(1);
        return (B) this;
    }
    
    private boolean depthBuffer;
    /**
    Set the value of the {@link javafx.scene.Scene#isDepthBuffer() depthBuffer} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B depthBuffer(boolean x) {
        this.depthBuffer = x;
        return (B) this;
    }
    
    private javafx.event.EventDispatcher eventDispatcher;
    /**
    Set the value of the {@link javafx.scene.Scene#getEventDispatcher() eventDispatcher} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B eventDispatcher(javafx.event.EventDispatcher x) {
        this.eventDispatcher = x;
        __set(2);
        return (B) this;
    }
    
    private javafx.scene.paint.Paint fill;
    /**
    Set the value of the {@link javafx.scene.Scene#getFill() fill} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B fill(javafx.scene.paint.Paint x) {
        this.fill = x;
        __set(3);
        return (B) this;
    }
    
    private double height = -1;
    /**
    Set the value of the {@link javafx.scene.Scene#getHeight() height} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B height(double x) {
        this.height = x;
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.ContextMenuEvent> onContextMenuRequested;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnContextMenuRequested() onContextMenuRequested} property for the instance constructed by this builder.
    * @since JavaFX 2.1
    */
    @SuppressWarnings("unchecked")
    public B onContextMenuRequested(javafx.event.EventHandler<? super javafx.scene.input.ContextMenuEvent> x) {
        this.onContextMenuRequested = x;
        __set(4);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> onDragDetected;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnDragDetected() onDragDetected} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onDragDetected(javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> x) {
        this.onDragDetected = x;
        __set(5);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.DragEvent> onDragDone;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnDragDone() onDragDone} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onDragDone(javafx.event.EventHandler<? super javafx.scene.input.DragEvent> x) {
        this.onDragDone = x;
        __set(6);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.DragEvent> onDragDropped;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnDragDropped() onDragDropped} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onDragDropped(javafx.event.EventHandler<? super javafx.scene.input.DragEvent> x) {
        this.onDragDropped = x;
        __set(7);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.DragEvent> onDragEntered;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnDragEntered() onDragEntered} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onDragEntered(javafx.event.EventHandler<? super javafx.scene.input.DragEvent> x) {
        this.onDragEntered = x;
        __set(8);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.DragEvent> onDragExited;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnDragExited() onDragExited} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onDragExited(javafx.event.EventHandler<? super javafx.scene.input.DragEvent> x) {
        this.onDragExited = x;
        __set(9);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.DragEvent> onDragOver;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnDragOver() onDragOver} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onDragOver(javafx.event.EventHandler<? super javafx.scene.input.DragEvent> x) {
        this.onDragOver = x;
        __set(10);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.InputMethodEvent> onInputMethodTextChanged;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnInputMethodTextChanged() onInputMethodTextChanged} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onInputMethodTextChanged(javafx.event.EventHandler<? super javafx.scene.input.InputMethodEvent> x) {
        this.onInputMethodTextChanged = x;
        __set(11);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.KeyEvent> onKeyPressed;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnKeyPressed() onKeyPressed} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onKeyPressed(javafx.event.EventHandler<? super javafx.scene.input.KeyEvent> x) {
        this.onKeyPressed = x;
        __set(12);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.KeyEvent> onKeyReleased;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnKeyReleased() onKeyReleased} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onKeyReleased(javafx.event.EventHandler<? super javafx.scene.input.KeyEvent> x) {
        this.onKeyReleased = x;
        __set(13);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.KeyEvent> onKeyTyped;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnKeyTyped() onKeyTyped} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onKeyTyped(javafx.event.EventHandler<? super javafx.scene.input.KeyEvent> x) {
        this.onKeyTyped = x;
        __set(14);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> onMouseClicked;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMouseClicked() onMouseClicked} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onMouseClicked(javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> x) {
        this.onMouseClicked = x;
        __set(15);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseDragEvent> onMouseDragEntered;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMouseDragEntered() onMouseDragEntered} property for the instance constructed by this builder.
    * @since JavaFX 2.1
    */
    @SuppressWarnings("unchecked")
    public B onMouseDragEntered(javafx.event.EventHandler<? super javafx.scene.input.MouseDragEvent> x) {
        this.onMouseDragEntered = x;
        __set(16);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseDragEvent> onMouseDragExited;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMouseDragExited() onMouseDragExited} property for the instance constructed by this builder.
    * @since JavaFX 2.1
    */
    @SuppressWarnings("unchecked")
    public B onMouseDragExited(javafx.event.EventHandler<? super javafx.scene.input.MouseDragEvent> x) {
        this.onMouseDragExited = x;
        __set(17);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> onMouseDragged;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMouseDragged() onMouseDragged} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onMouseDragged(javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> x) {
        this.onMouseDragged = x;
        __set(18);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseDragEvent> onMouseDragOver;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMouseDragOver() onMouseDragOver} property for the instance constructed by this builder.
    * @since JavaFX 2.1
    */
    @SuppressWarnings("unchecked")
    public B onMouseDragOver(javafx.event.EventHandler<? super javafx.scene.input.MouseDragEvent> x) {
        this.onMouseDragOver = x;
        __set(19);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseDragEvent> onMouseDragReleased;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMouseDragReleased() onMouseDragReleased} property for the instance constructed by this builder.
    * @since JavaFX 2.1
    */
    @SuppressWarnings("unchecked")
    public B onMouseDragReleased(javafx.event.EventHandler<? super javafx.scene.input.MouseDragEvent> x) {
        this.onMouseDragReleased = x;
        __set(20);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> onMouseEntered;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMouseEntered() onMouseEntered} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onMouseEntered(javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> x) {
        this.onMouseEntered = x;
        __set(21);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> onMouseExited;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMouseExited() onMouseExited} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onMouseExited(javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> x) {
        this.onMouseExited = x;
        __set(22);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> onMouseMoved;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMouseMoved() onMouseMoved} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onMouseMoved(javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> x) {
        this.onMouseMoved = x;
        __set(23);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> onMousePressed;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMousePressed() onMousePressed} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onMousePressed(javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> x) {
        this.onMousePressed = x;
        __set(24);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> onMouseReleased;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnMouseReleased() onMouseReleased} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onMouseReleased(javafx.event.EventHandler<? super javafx.scene.input.MouseEvent> x) {
        this.onMouseReleased = x;
        __set(25);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.RotateEvent> onRotate;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnRotate() onRotate} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onRotate(javafx.event.EventHandler<? super javafx.scene.input.RotateEvent> x) {
        this.onRotate = x;
        __set(26);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.RotateEvent> onRotationFinished;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnRotationFinished() onRotationFinished} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onRotationFinished(javafx.event.EventHandler<? super javafx.scene.input.RotateEvent> x) {
        this.onRotationFinished = x;
        __set(27);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.RotateEvent> onRotationStarted;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnRotationStarted() onRotationStarted} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onRotationStarted(javafx.event.EventHandler<? super javafx.scene.input.RotateEvent> x) {
        this.onRotationStarted = x;
        __set(28);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.ScrollEvent> onScroll;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnScroll() onScroll} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B onScroll(javafx.event.EventHandler<? super javafx.scene.input.ScrollEvent> x) {
        this.onScroll = x;
        __set(29);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.ScrollEvent> onScrollFinished;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnScrollFinished() onScrollFinished} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onScrollFinished(javafx.event.EventHandler<? super javafx.scene.input.ScrollEvent> x) {
        this.onScrollFinished = x;
        __set(30);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.ScrollEvent> onScrollStarted;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnScrollStarted() onScrollStarted} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onScrollStarted(javafx.event.EventHandler<? super javafx.scene.input.ScrollEvent> x) {
        this.onScrollStarted = x;
        __set(31);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.SwipeEvent> onSwipeDown;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnSwipeDown() onSwipeDown} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onSwipeDown(javafx.event.EventHandler<? super javafx.scene.input.SwipeEvent> x) {
        this.onSwipeDown = x;
        __set(32);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.SwipeEvent> onSwipeLeft;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnSwipeLeft() onSwipeLeft} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onSwipeLeft(javafx.event.EventHandler<? super javafx.scene.input.SwipeEvent> x) {
        this.onSwipeLeft = x;
        __set(33);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.SwipeEvent> onSwipeRight;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnSwipeRight() onSwipeRight} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onSwipeRight(javafx.event.EventHandler<? super javafx.scene.input.SwipeEvent> x) {
        this.onSwipeRight = x;
        __set(34);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.SwipeEvent> onSwipeUp;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnSwipeUp() onSwipeUp} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onSwipeUp(javafx.event.EventHandler<? super javafx.scene.input.SwipeEvent> x) {
        this.onSwipeUp = x;
        __set(35);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.TouchEvent> onTouchMoved;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnTouchMoved() onTouchMoved} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onTouchMoved(javafx.event.EventHandler<? super javafx.scene.input.TouchEvent> x) {
        this.onTouchMoved = x;
        __set(36);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.TouchEvent> onTouchPressed;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnTouchPressed() onTouchPressed} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onTouchPressed(javafx.event.EventHandler<? super javafx.scene.input.TouchEvent> x) {
        this.onTouchPressed = x;
        __set(37);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.TouchEvent> onTouchReleased;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnTouchReleased() onTouchReleased} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onTouchReleased(javafx.event.EventHandler<? super javafx.scene.input.TouchEvent> x) {
        this.onTouchReleased = x;
        __set(38);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.TouchEvent> onTouchStationary;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnTouchStationary() onTouchStationary} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onTouchStationary(javafx.event.EventHandler<? super javafx.scene.input.TouchEvent> x) {
        this.onTouchStationary = x;
        __set(39);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.ZoomEvent> onZoom;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnZoom() onZoom} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onZoom(javafx.event.EventHandler<? super javafx.scene.input.ZoomEvent> x) {
        this.onZoom = x;
        __set(40);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.ZoomEvent> onZoomFinished;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnZoomFinished() onZoomFinished} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onZoomFinished(javafx.event.EventHandler<? super javafx.scene.input.ZoomEvent> x) {
        this.onZoomFinished = x;
        __set(41);
        return (B) this;
    }
    
    private javafx.event.EventHandler<? super javafx.scene.input.ZoomEvent> onZoomStarted;
    /**
    Set the value of the {@link javafx.scene.Scene#getOnZoomStarted() onZoomStarted} property for the instance constructed by this builder.
    * @since JavaFX 2.2
    */
    @SuppressWarnings("unchecked")
    public B onZoomStarted(javafx.event.EventHandler<? super javafx.scene.input.ZoomEvent> x) {
        this.onZoomStarted = x;
        __set(42);
        return (B) this;
    }
    
    private javafx.scene.Parent root;
    /**
    Set the value of the {@link javafx.scene.Scene#getRoot() root} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B root(javafx.scene.Parent x) {
        this.root = x;
        return (B) this;
    }
    
    private java.util.Collection<? extends java.lang.String> stylesheets;
    /**
    Add the given items to the List of items in the {@link javafx.scene.Scene#getStylesheets() stylesheets} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B stylesheets(java.util.Collection<? extends java.lang.String> x) {
        this.stylesheets = x;
        __set(43);
        return (B) this;
    }
    
    /**
    Add the given items to the List of items in the {@link javafx.scene.Scene#getStylesheets() stylesheets} property for the instance constructed by this builder.
    */
    public B stylesheets(java.lang.String... x) {
        return stylesheets(java.util.Arrays.asList(x));
    }
    
    private double width = -1;
    /**
    Set the value of the {@link javafx.scene.Scene#getWidth() width} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B width(double x) {
        this.width = x;
        return (B) this;
    }
    
    /**
    Make an instance of {@link javafx.scene.Scene} based on the properties set on this builder.
    */
    public javafx.scene.Scene build() {
        javafx.scene.Scene x = new javafx.scene.Scene(this.root, this.width, this.height, this.depthBuffer);
        applyTo(x);
        return x;
    }
}
