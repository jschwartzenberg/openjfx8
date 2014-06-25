/*
 * Copyright (c) 2014, Oracle and/or its affiliates. All rights reserved.
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

package com.sun.glass.ui.monocle.input;

import com.sun.glass.ui.monocle.input.devices.TestTouchDevice;
import com.sun.glass.ui.monocle.input.devices.TestTouchDevices;
import com.sun.javafx.PlatformUtil;
import org.junit.*;
import org.junit.runners.Parameterized;

import java.util.Collection;

/**
 * Rotate tests generated by two touch points.
 * By default rotation starting form 0 degrees position - on Axis Y,
 * but it can be sent from any other location on the object
 *  */
public class RotateTest extends ParameterizedTestBase {

    private int newX1;
    private int newY1;
    private static final int ZERO_ANGLE = 0;
    private int centerX;
    private int centerY;
    private int radius;
    private int p1;
    private int p2;

    public RotateTest(TestTouchDevice device) {
        super(device);
    }

    @Parameterized.Parameters
    public static Collection<Object[]> data() {
        return TestTouchDevices.getTouchDeviceParameters(2);
    }

    @Before
    public void init() {
        Assume.assumeTrue(!PlatformUtil.isMac());
        Assume.assumeTrue(!PlatformUtil.isWindows());
        //Rotate tests should be run only on platforms that support current feature
        Assume.assumeTrue(Boolean.getBoolean("com.sun.javafx.gestures.rotate"));
        centerX = (int) Math.round(width * 0.5);
        centerY = (int) Math.round(height * 0.5);
        radius = (int) Math.round(height * 0.45);
    }

    @After
    public void releaseAll() throws Exception {
        if (device.getPressedPoints() == 2) {
            TestLog.reset();
            device.removePoint(p1);
            device.removePoint(p2);
            device.sync();
        }
    }

    private void updateNewTouchPoint(int angle, int radius, int centerX, int centerY) {

        int transformedAngle = 90 - angle;
        newX1 = centerX + (int) Math.round(radius *
                Math.cos(Math.toRadians(transformedAngle)));
        newY1 = centerY - (int) Math.round(radius *
                Math.sin(Math.toRadians(transformedAngle)));
    }

    private int getDistance(int xPoint1, int yPoint1, int xPoint2, int yPoint2) {
        double d = Math.sqrt(Math.pow((xPoint1 - xPoint2), 2)
                + Math.pow((yPoint1 - yPoint2), 2));
        return (int) d;
    }

    private int getRotateThreshold() {
        String s = System.getProperty("com.sun.javafx.gestures.rotate.threshold");
        if (s != null) {
            return Integer.valueOf(s);
        } else {
            return 5;
        }
    }

    private void Rotate(int startAngle, int radius, int x2, int y2, int angleStep,
                        int numOfIterations) throws Exception {

        int totalAngle = angleStep;
        updateNewTouchPoint(startAngle, radius, x2, y2);

        TestLog.reset();
        p1 = device.addPoint(newX1, newY1);
        p2 = device.addPoint(x2, y2);
        device.sync();
        //verify pressing two fingers
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", newX1, newY1);
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x2, y2);

        //saving previous coordinates:
        int previousX = newX1;
        int previousY = newY1;

        updateNewTouchPoint((angleStep + startAngle), radius, x2, y2);

        Assume.assumeTrue(getDistance(previousX, previousY, newX1, newY1 )
                > device.getTapRadius());

        //start the rotation
        TestLog.reset();
        device.setPoint(p1, newX1, newY1);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d", newX1, newY1);
        TestLog.waitForLogContaining("TouchPoint: STATIONARY %d, %d", x2, y2);

        if (Math.abs(angleStep) >= getRotateThreshold()) {
            TestLog.waitForLogContaining("Rotation started, angle: " + ZERO_ANGLE
                + ", total angle: " + ZERO_ANGLE + ", inertia value: false");
            TestLog.waitForLogContaining("Rotation, angle: " + angleStep
                + ", total angle: " + totalAngle
                + ", inertia value: false");
        } else {
            Assert.assertEquals(0, TestLog.countLogContaining("Rotation started"));
            Assert.assertEquals(0, TestLog.countLogContaining("Rotation, angle"));
        }
        boolean passedTheThreshold =false;
        if (numOfIterations >= 2) {
            for (int i = 2; i <= numOfIterations; i++) {
                updateNewTouchPoint(angleStep * i + startAngle, radius, x2, y2);
                totalAngle += angleStep;
                TestLog.reset();
                device.setPoint(p1, newX1, newY1);
                device.sync();

                TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d", newX1, newY1);
                TestLog.waitForLogContaining("TouchPoint: STATIONARY %d, %d", x2, y2);

                String expectedLog;
                if (Math.abs(angleStep) < getRotateThreshold()) {
                    if(Math.abs(totalAngle) >= getRotateThreshold()) {
                        if (!passedTheThreshold) {
                            expectedLog = "Rotation, angle: " + totalAngle
                                + ", total angle: " + totalAngle
                                + ", inertia value: false";
                            passedTheThreshold = true;
                        } else {
                            expectedLog = "Rotation, angle: " + angleStep
                                + ", total angle: " + totalAngle
                                + ", inertia value: false";
                        }
                    } else {
                        expectedLog = "sync";
                    }
                } else {
                    expectedLog = "Rotation, angle: " + angleStep
                            + ", total angle: " + totalAngle
                            + ", inertia value: false";
                }
                TestLog.waitForLogContaining(expectedLog);
            }
        }
        TestLog.reset();
        device.removePoint(p1);
        device.removePoint(p2);
        device.sync();
        //verify fingers release
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", newX1, newY1);
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x2, y2);
        if (Math.abs(totalAngle) >= getRotateThreshold()) {
            TestLog.waitForLogContaining("Rotation finished, angle: " + ZERO_ANGLE
                    + ", total angle: " + totalAngle + ", inertia value: false");
            Assert.assertEquals(1, TestLog.countLogContaining("Rotation "
                    + "finished, " + "angle: " + ZERO_ANGLE
                    + ", total angle: " + totalAngle
                    + ", inertia value: false"));
        } else {
            Assert.assertEquals(0, TestLog.countLogContaining("Rotation finished, "
                    + "angle: " + ZERO_ANGLE + ", total angle: " + totalAngle
                    + ", inertia value: false"));
        }
        if (TestLog.countLogContaining("Rotation finished") > 0) {
            TestLog.waitForLogContainingSubstrings("Rotation", "inertia value: true");
        }
        TestLog.reset();
        p2 = device.addPoint(x2, y2);
        device.sync();
        device.removePoint(p2);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x2, y2);
    }

    private void Rotate(int radius, int x2, int y2, int angleStep,
                        int numOfIterations) throws Exception {
        Rotate(0, radius, x2, y2, angleStep, numOfIterations);
    }

    private void Rotate(int startAngle, int angleStep, int numOfIterations) throws Exception {
        Rotate(startAngle, radius, centerX, centerY, angleStep, numOfIterations);
    }

    private void Rotate(int angleStep, int numOfIterations) throws Exception {
        Rotate(0, radius, centerX, centerY, angleStep, numOfIterations);
    }

    /**
     * Tap two fingers, drag a little bit upper finger right in order move,
     * but not enough for rotation.
     */
    @Test
    public void testSmallStepRightNoRotateSent() throws Exception {
        Rotate(4, 1);
    }

    /**
     * Tap two fingers, drag a little bit upper finger right in order move,
     * but not enough for rotation, then make 2 more small moves
     */
    @Test
    public void testRotateRightByFewSmallSteps() throws Exception {
        Rotate(4, 5);
    }

    /**
     * Tap two fingers, drag upper finger right in order to rotate
     */
    @Test
    public void testRotateRight() throws Exception {
        Rotate(15, 6);
    }

    /**
     * Tap two fingers, rotate the object right (by 3 steps, 50 degrees each)
     */
    @Test
    public void testRotateRightBigSteps() throws Exception {
        Rotate(50, 3);
    }

    /**
     * Tap two fingers, rotate the object right by only 1 very big step - 80 degrees
     */
    @Test
    @Ignore //RT-36616
    public void testRotateRightOneBigStep() throws Exception {
        Rotate(80, 1);
    }

    /**
     * Tap two fingers, drag a little bit upper finger left in order move,
     * but not enough for rotation.
     */
    @Test
    public void testSmallStepLeftNoRotateSent() throws Exception {
        Rotate(-4, 1);
    }

    /**
     * Tap two fingers, drag a little bit upper finger left in order move,
     * but not enough for rotation, then make 9 more small moves
     */
    @Test
    public void testRotateLeftByFewSmallSteps() throws Exception {
        Rotate(-4, 10);
    }

    /**
     * Tap two fingers, drag upper finger left in order to rotate
     */
    @Test
    public void testRotateLeft() throws Exception {
        Rotate(-10, 4);
    }

    /**
     * Tap two fingers, rotate the object left (by 5 steps, 40 degrees each)
     */
    @Test
    public void testRotateLeftBigSteps() throws Exception {
        Rotate(-40, 5);
    }

    /**
     * Tap two fingers, rotate the object left by only 1 very big step - 70 degrees
     */
    @Test
    @Ignore //RT-36616
    public void testRotateLeftOneBigStep() throws Exception {
        Rotate(-70, 1);
    }

    /**
     * Tap two fingers in 45 degrees, rotate the object right
     */
    @Test
    public void testRotateRightFrom45Degrees() throws Exception {
        Rotate(45, 20, 3);
    }

    /**
     * Tap two fingers in 45 degrees, rotate the object left
     */
    @Test
    public void testRotateLeftFrom45Degrees() throws Exception {
        Rotate(45, -20, 3);
    }

    /**
     * Tap two fingers in -45 degrees, rotate the object right
     */
    @Test
    public void testRotateRightFromMinus45Degrees() throws Exception {
        Rotate(-45, 20, 3);
    }

    /**
     * Tap two fingers in -45 degrees, rotate the object left
     */
    @Test
    public void testRotateLeftFromMinus45Degrees() throws Exception {
        Rotate(-45, -20, 3);
    }

    /**
     * Tap two fingers in 140 degrees, rotate the object right
     */
    @Test
    public void testRotateRightFrom140Degrees() throws Exception {
        Rotate(140, 20, 3);
    }

    /**
     * Tap two fingers in 140 degrees, rotate the object left
     */
    @Test
    public void testRotateLeftFrom140Degrees() throws Exception {
        Rotate(140, -20, 3);
    }

    /**
     * Tap two fingers in -140 degrees, rotate the object right
     */
    @Test
    public void testRotateRightFromMinus140Degrees() throws Exception {
        Rotate(-140, 20, 3);
    }

    /**
     * Tap two fingers in -140 degrees, rotate the object left
     */
    @Test
    public void testRotateLeftFromMinus140Degrees() throws Exception {
        Rotate(-140, -20, 3);
    }

}