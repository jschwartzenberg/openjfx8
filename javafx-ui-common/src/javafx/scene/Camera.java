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

package javafx.scene;

import com.sun.javafx.geom.BaseBounds;
import com.sun.javafx.geom.BoxBounds;
import com.sun.javafx.geom.PickRay;
import com.sun.javafx.geom.Vec3d;
import com.sun.javafx.geom.transform.Affine3D;
import com.sun.javafx.geom.transform.BaseTransform;
import com.sun.javafx.geom.transform.GeneralTransform3D;
import com.sun.javafx.geom.transform.NoninvertibleTransformException;
import com.sun.javafx.jmx.MXNodeAlgorithm;
import com.sun.javafx.jmx.MXNodeAlgorithmContext;
import com.sun.javafx.scene.CameraAccess;
import com.sun.javafx.scene.DirtyBits;
import com.sun.javafx.sg.PGCamera;
import javafx.beans.InvalidationListener;
import javafx.beans.Observable;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.geometry.Point3D;
import javafx.scene.transform.Transform;
import sun.util.logging.PlatformLogger;


/**
 * Base class for a camera used to render a scene.
 *
 * @since JavaFX 1.3
 */
public abstract class Camera extends Node {
   
    private Affine3D localToSceneTx = new Affine3D();

    protected Camera() {
        InvalidationListener dirtyTransformListener = new InvalidationListener() {
            @Override
            public void invalidated(Observable observable) {
                impl_markDirty(DirtyBits.NODE_CAMERA_TRANSFORM);
            }
        };

        this.localToSceneTransformProperty().addListener(dirtyTransformListener);
        // if camera is removed from scene it needs to stop using its transforms
        this.sceneProperty().addListener(dirtyTransformListener);
    }

    // NOTE: farClipInScene and nearClipInScene are valid only if there is no rotation
    private double farClipInScene;
    private double nearClipInScene;

    // only one of them can be non-null at a time
    private Scene ownerScene = null;
    private SubScene ownerSubScene = null;

    private GeneralTransform3D projViewTx = new GeneralTransform3D();
    private GeneralTransform3D projTx = new GeneralTransform3D();
    private Affine3D viewTx = new Affine3D();
    private double viewWidth = 1.0;
    private double viewHeight = 1.0;
    private Vec3d position = new Vec3d();

    private boolean clipInSceneValid = false;
    private boolean projViewTxValid = false;
    private boolean localToSceneValid = false;
    private boolean sceneToLocalValid = false;

    double getFarClipInScene() {
        updateClipPlane();
        return farClipInScene;
    }

    double getNearClipInScene() {
        updateClipPlane();
        return nearClipInScene;
    }

    private void updateClipPlane() {
        if (!clipInSceneValid) {
            final Transform localToSceneTransform = getLocalToSceneTransform();
            nearClipInScene = localToSceneTransform.transform(0, 0, getNearClip()).getZ();
            farClipInScene = localToSceneTransform.transform(0, 0, getFarClip()).getZ();
            clipInSceneValid = true;
        }
    }

    /**
     * An affine transform that holds the computed scene-to-local transform.
     * It is used to convert node to camera coordinate when rotation is involved.
     */
    private Affine3D sceneToLocalTx = new Affine3D();

    Affine3D getSceneToLocalTransform() {
        if (!sceneToLocalValid) {
            sceneToLocalTx.setTransform(getCameraTransform());
            try {
                sceneToLocalTx.invert();
            } catch (NoninvertibleTransformException ex) {
                String logname = Camera.class.getName();
                PlatformLogger.getLogger(logname).severe("getSceneToLocalTransform", ex);
                sceneToLocalTx.setToIdentity();
            }
            sceneToLocalValid = true;
        }

        return sceneToLocalTx;
    }

    /**
     * Specifies the near clipping plane of this {@code Camera} in the local
     * coordinate system of this node.
     *
     * @defaultValue 0.1
     * @since JavaFX 8
     */
    private DoubleProperty nearClip;

    public final void setNearClip(double value){
        nearClipProperty().set(value);
    }

    public final double getNearClip() {
        return nearClip == null ? 0.1 : nearClip.get();
    }

    public final DoubleProperty nearClipProperty() {
        if (nearClip == null) {
            nearClip = new SimpleDoubleProperty(Camera.this, "nearClip", 0.1) {
                @Override
                protected void invalidated() {
                    clipInSceneValid = false;
                    impl_markDirty(DirtyBits.NODE_CAMERA);
                }
            };
        }
        return nearClip;
    }

    /**
     * Specifies the far clipping plane of this {@code Camera} in the local
     * coordinate system of this node.
     * <p>
     *
     * @defaultValue 100.0
     * @since JavaFX 8
     */
    private DoubleProperty farClip;

    public final void setFarClip(double value){
        farClipProperty().set(value);
    }

    public final double getFarClip() {
        return farClip == null ? 100.0 : farClip.get();
    }

    public final DoubleProperty farClipProperty() {
        if (farClip == null) {
            farClip = new SimpleDoubleProperty(Camera.this, "farClip", 100.0) {
                @Override
                protected void invalidated() {
                    clipInSceneValid = false;
                    impl_markDirty(DirtyBits.NODE_CAMERA);
                }
            };
        }
        return farClip;
    }
    
    PGCamera getPlatformCamera() {
        return (PGCamera) impl_getPGNode();
    }

    Camera copy() {
        return this;
    }

    /**
     * @treatAsPrivate implementation detail
     * @deprecated This is an internal API that is not intended for use and will be removed in the next version
     */
    @Deprecated
    @Override
    public void impl_updatePG() {
        super.impl_updatePG();
        PGCamera pgCamera = (PGCamera)impl_getPGNode();
        if (!impl_isDirtyEmpty()) {
            if (impl_isDirty(DirtyBits.NODE_CAMERA)) {
                pgCamera.setNearClip((float) getNearClip());
                pgCamera.setFarClip((float) getFarClip());
                pgCamera.setViewWidth(getViewWidth());
                pgCamera.setViewHeight(getViewHeight());
            }
            if (impl_isDirty(DirtyBits.NODE_CAMERA_TRANSFORM)) {
                // TODO: 3D - For now, we are treating the scene as world.
                // This may need to change for the fixed eye position case.
                pgCamera.setWorldTransform(getCameraTransform());
            }

            pgCamera.setProjViewTransform(getProjViewTransform());

            position = computePosition(position);
            getCameraTransform().transform(position, position);
            pgCamera.setPosition(position);
        }
    }

    void setViewWidth(double width) {
        this.viewWidth = width;
        impl_markDirty(DirtyBits.NODE_CAMERA);
    }

    double getViewWidth() {
        return viewWidth;
    }

    void setViewHeight(double height) {
        this.viewHeight = height;
        impl_markDirty(DirtyBits.NODE_CAMERA);
    }

    double getViewHeight() {
        return viewHeight;
    }

    void setOwnerScene(Scene s) {
        if (s == null) {
            ownerScene = null;
        } else if (s != ownerScene) {
            if (ownerScene != null || ownerSubScene != null) {
                throw new IllegalArgumentException(this
                        + "is already set as camera in other scene or subscene");
            }
            ownerScene = s;
            markOwnerDirty();
        }
    }

    void setOwnerSubScene(SubScene s) {
        if (s == null) {
            ownerSubScene = null;
        } else if (s != ownerSubScene) {
            if (ownerScene != null || ownerSubScene != null) {
                throw new IllegalArgumentException(this
                        + "is already set as camera in other scene or subscene");
            }
            ownerSubScene = s;
            markOwnerDirty();
        }
    }

    @Override
    protected void impl_markDirty(DirtyBits dirtyBit) {
        super.impl_markDirty(dirtyBit);
        if (dirtyBit == DirtyBits.NODE_CAMERA_TRANSFORM) {
            localToSceneValid = false;
            sceneToLocalValid = false;
            clipInSceneValid = false;
            projViewTxValid = false;
        } else if (dirtyBit == DirtyBits.NODE_CAMERA) {
            projViewTxValid = false;
        }
        markOwnerDirty();
    }

    private void markOwnerDirty() {
        // if the camera is part of the scene/subScene, we don't need to notify
        // the owner as the camera will be added to its dirty list as usual

        if (ownerScene != null && ownerScene != getScene()) {
            ownerScene.markCameraDirty();
        }
        if (ownerSubScene != null && ownerSubScene != getSubScene()) {
            ownerSubScene.markCameraDirty();
        }
    }

    /**
     * Returns the local-to-scene transform of this camera.
     * Package private, for use in our internal subclasses.
     * Returns directly the internal instance, it must not be altered.
     */
    Affine3D getCameraTransform() {
        if (!localToSceneValid) {
            localToSceneTx.setToIdentity();
            // if the camera is not in scene, its transforms are ignored
            if (getScene() != null) {
                getLocalToSceneTransform().impl_apply(localToSceneTx);
            }
            localToSceneValid = true;
        }
        return localToSceneTx;
    }

    abstract void computeProjectionTransform(GeneralTransform3D proj);
    abstract void computeViewTransform(Affine3D view);

    /**
     * Returns the projView transform of this camera.
     * Package private, for internal use.
     * Returns directly the internal instance, it must not be altered.
     */
    GeneralTransform3D getProjViewTransform() {
        if (!projViewTxValid) {
            computeProjectionTransform(projTx);
            computeViewTransform(viewTx);

            projViewTx.set(projTx);
            projViewTx.mul(viewTx);
            projViewTx.mul(getSceneToLocalTransform());

            projViewTxValid = true;
        }

        return projViewTx;
    }

    /**
     * Transforms the given 3D point to the flat projected coordinates.
     */
    private Point3D project(Point3D p) {

        final Vec3d vec = getProjViewTransform().transform(new Vec3d(
                p.getX(), p.getY(), p.getZ()));

        final double halfViewWidth = getViewWidth() / 2.0;
        final double halfViewHeight = getViewHeight() / 2.0;

        return new Point3D(
                halfViewWidth * (1 + vec.x),
                halfViewHeight * (1 - vec.y),
                0.0);
    }

    /**
     * Computes intersection point of the pick ray cast by the given coordinates
     * and the node's local XY plane.
     */
    private Point3D pickNodeXYPlane(Node node, double x, double y) {
        final PickRay ray = computePickRay(x, y, null);

        final Affine3D localToScene = new Affine3D();
        node.getLocalToSceneTransform().impl_apply(localToScene);

        final Vec3d o = ray.getOriginNoClone();
        final Vec3d d = ray.getDirectionNoClone();

        try {
            localToScene.inverseTransform(o, o);
            localToScene.inverseDeltaTransform(d, d);
        } catch (NoninvertibleTransformException e) {
            return null;
        }

        if (almostZero(d.z)) {
            return null;
        }

        final double t = -o.z / d.z;
        return new Point3D(o.x + (d.x * t), o.y + (d.y * t), 0.0);
    }

    /**
     * Computes intersection point of the pick ray cast by the given coordinates
     * and the projection plane.
     */
    private Point3D pickProjectPlane(double x, double y) {
        final PickRay ray = computePickRay(x, y, null);
        final Vec3d p = new Vec3d();
        p.add(ray.getOriginNoClone(), ray.getDirectionNoClone());

        return new Point3D(p.x, p.y, p.z);
    }


    /**
     * Computes pick ray for the content rendered by this camera.
     * @param x horizontal coordinate of the pick ray in the projected
     *               view, usually mouse cursor position
     * @param y vertical coordinate of the pick ray in the projected
     *               view, usually mouse cursor position
     * @param pickRay pick ray to be reused. New instance is created in case
     *                of null.
     * @return The PickRay instance computed based on this camera and the given
     *         arguments.
     */
    abstract PickRay computePickRay(double x, double y, PickRay pickRay);

    /**
     * Computes local position of the camera in the scene.
     * @param position Position to be reused. New instance is created in case 
     *                 of null.
     * @return The position of the camera in the scene in camera local coords
     */
    abstract Vec3d computePosition(Vec3d position);

    /**
     * @treatAsPrivate implementation detail
     * @deprecated This is an internal API that is not intended for use and will be removed in the next version
     */
    @Deprecated
    @Override
    public BaseBounds impl_computeGeomBounds(BaseBounds bounds, BaseTransform tx) {
        return new BoxBounds(0, 0, 0, 0, 0, 0);
    }

    /**
     * @treatAsPrivate implementation detail
     * @deprecated This is an internal API that is not intended for use and will be removed in the next version
     */
    @Deprecated
    @Override
    protected boolean impl_computeContains(double localX, double localY) {
        return false;
    }

    /**
     * @treatAsPrivate implementation detail
     * @deprecated This is an internal API that is not intended for use and will be removed in the next version
     */
    @Deprecated
    @Override
    public Object impl_processMXNode(MXNodeAlgorithm alg, MXNodeAlgorithmContext ctx) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    /**
     * This class is used by classes in different packages to get access to
     * private and package private methods.
     */
    private static class CameraAccessImpl extends CameraAccess {

        @Override
        public Point3D project(Camera camera, Point3D p) {
            return camera.project(p);
        }

        @Override
        public Point3D pickNodeXYPlane(Camera camera, Node node, double x, double y) {
            return camera.pickNodeXYPlane(node, x, y);
        }

        @Override
        public Point3D pickProjectPlane(Camera camera, double x, double y) {
            return camera.pickProjectPlane(x, y);
        }
    }

    static {
        CameraAccess.setCameraAccess(new CameraAccessImpl());
    }
}
