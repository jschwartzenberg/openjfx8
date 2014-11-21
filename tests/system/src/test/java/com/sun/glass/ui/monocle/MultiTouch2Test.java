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

package com.sun.glass.ui.monocle;

import com.sun.glass.ui.monocle.input.devices.TestTouchDevice;
import com.sun.glass.ui.monocle.input.devices.TestTouchDevices;
import org.junit.Test;
import org.junit.runners.Parameterized;

import java.util.Collection;

/** Multitouch tests with two points */
public class MultiTouch2Test extends ParameterizedTestBase {

    public MultiTouch2Test(TestTouchDevice device) {
        super(device);
    }

    @Parameterized.Parameters
    public static Collection<Object[]> data() {
        return TestTouchDevices.getTouchDeviceParameters(2);
    }

    @Test
    public void twoFingerTap() throws Exception {
        final int x1 = (int) Math.round(width * 0.5f);
        final int y1 = (int) Math.round(height * 0.5f);
        final int x2 = (int) Math.round(width * 0.75f);
        final int y2 = (int) Math.round(height * 0.75f);
        TestLog.reset();
        // first finger
        int p1 = device.addPoint(x1, y1);
        // second finger
        int p2 = device.addPoint(x2, y2);
        device.sync();
        TestLog.waitForLog("Touch pressed: "
                                   + x1 + ", " + y1, 3000);
        TestLog.waitForLogContaining("TouchPoint: PRESSED " + x1 + ", " + y1, 3000);
        TestLog.waitForLogContaining("TouchPoint: PRESSED " + x2 + ", " + y2, 3000);
        TestLog.reset();
        // release
        device.removePoint(p1);
        device.removePoint(p2);
        device.sync();
        TestLog.waitForLog("Touch released: " + x1 + ", " + y1, 3000);
    }

    /**
     * Touch down two fingers, release first, release second
     */
    @Test
    public void pressTwoFingersReleaseOne() throws Exception {
        final int x1 = (int) Math.round(width / 8.0);
        final int y1 = (int) Math.round(height / 8.0);
        final int x2 = (int) Math.round(width / 5.0);
        final int y2 = (int) Math.round(height / 5.0);
        final int x3 = (int) Math.round(width / 3.0);
        final int y3 = (int) Math.round(height / 3.0);

        TestLog.reset();
        //press first finger
        int p1 = device.addPoint(x1, y1);
        //add a second finger
        int p2 = device.addPoint(x2, y2);
        device.sync();
        TestLog.waitForLog("Mouse pressed: " + x1 + ", " + y1, 3000l);
        TestLog.waitForLog("Touch pressed: " + x1 + ", " + y1, 3000l);
        TestLog.waitForLogContaining("TouchPoint: PRESSED " + x1 + ", " + y1, 3000l);
        TestLog.waitForLogContaining("TouchPoint: PRESSED " + x2 + ", " + y2, 3000l);
        TestLog.waitForLog("Touch pressed: " + x2 + ", " + y2, 3000l);

        //release one finger
        TestLog.reset();
        device.removePoint(p1);
        device.sync();
        TestLog.waitForLog("Touch released: " + x1 + ", " + y1, 3000l);
        TestLog.waitForLogContaining("TouchPoint: RELEASED " + x1 + ", " + y1, 3000l);
        TestLog.waitForLogContaining("TouchPoint: STATIONARY " + x2 + ", " + y2, 3000l);

        TestLog.reset();
        device.setPoint(p2, x3, y3);
        device.sync();
        TestLog.waitForLog("Mouse dragged: " + x3 + ", " + y3, 3000l);

        //release second finger
        TestLog.reset();
        device.removePoint(p2);
        device.sync();
        TestLog.waitForLogContaining("Mouse released: " + x3 + ", " + y3, 3000l);
        TestLog.waitForLogContaining("TouchPoint: RELEASED " + x3 + ", " + y3, 3000l);
    }

    @Test
//    @Ignore("RT-35546")
    public void twoFingerDrag() throws Exception {
        final int x1 = (int) Math.round(width * 0.5f);
        final int y1 = (int) Math.round(height * 0.5f);
        final int x2 = (int) Math.round(width * 0.75f);
        final int y2 = (int) Math.round(height * 0.75f);
        final int dx = device.getTapRadius();
        final int dy = device.getTapRadius();
        // first finger
        int p1 = device.addPoint(x1, y1);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x1, y1);
        // add a second finger
        int p2 = device.addPoint(x2, y2);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: STATIONARY %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x2, y2);
        // drag both fingers
        for (int i = 1; i < 10; i++) {
            TestLog.reset();
            device.setPoint(p1, x1 + dx * i, y1 + dy * i);
            device.setPoint(p2, x2 + dx * i, y2 + dy * i);
            device.sync();
            TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d",
                                         x1 + dx * i, y1 + dy * i);
            TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d",
                                         x2 + dx * i, y2 + dy * i);
        }
        for (int i = 8; i >= 0; i--) {
            TestLog.reset();
            device.setPoint(p1, x1 + dx * i, y1 + dy * i);
            device.setPoint(p2, x2 + dx * i, y2 + dy * i);
            device.sync();
            TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d",
                                         x1 + dx * i, y1 + dy * i);
            TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d",
                                         x2 + dx * i, y2 + dy * i);
        }
        TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d", x2, y2);
        //release first finger
        TestLog.reset();
        device.removePoint(p1);
        device.sync();
        TestLog.waitForLogContaining("Touch released: %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: STATIONARY %d, %d", x2, y2);
        //release second finger
        TestLog.reset();
        device.removePoint(p2);
        device.sync();
        TestLog.waitForLogContaining("Touch released: %d, %d", x2, y2);
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x2, y2);
        TestLog.reset();
        // tap to cancel inertia
        int p3 = device.addPoint(x1, y1);
        device.sync();
        device.removePoint(p3);
        device.sync();
        TestLog.waitForLog("Mouse clicked: %d, %d", x1, y1);
    }

    /**
     * Touch down two fingers, release both,
     * touch down two fingers again and release them
     */
    @Test
    public void pressReleasePressTest() throws Exception {
        int x1 = (int) Math.round(width / 2);
        int y1 = (int) Math.round(height * 0.3);
        int x2 = (int) Math.round(width / 2);
        int y2 = (int) Math.round(height * 0.7);

        //press two fingers
        TestLog.reset();
        int p1 = device.addPoint(x1, y1);
        int p2 = device.addPoint(x2, y2);
        device.sync();

        //verify pressing two fingers
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x2, y2);

        //release both fingers
        TestLog.reset();
        device.removePoint(p1);
        device.removePoint(p2);
        device.sync();

        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x2, y2);

        //press two fingers second time
        TestLog.reset();
        p1 = device.addPoint(x1, y1);
        p2 = device.addPoint(x2, y2);
        device.sync();

        //verify pressing two fingers
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x2, y2);

        //release both fingers
        TestLog.reset();
        device.removePoint(p1);
        device.removePoint(p2);
        device.sync();

        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x2, y2);
    }

    @Test
    public void twoFingerDragSingleFingerTap() throws Exception {
        final int x1 = (int) Math.round(width * 0.5f);
        final int y1 = (int) Math.round(height * 0.5f);
        final int x2 = (int) Math.round(width * 0.75f);
        final int y2 = (int) Math.round(height * 0.75f);
        final int dx = device.getTapRadius();
        final int dy = device.getTapRadius();
        // first finger
        int p1 = device.addPoint(x1, y1);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x1, y1);
        // add a second finger
        int p2 = device.addPoint(x2, y2);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: STATIONARY %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x2, y2);
        // drag both fingers
        device.setPoint(p1, x1 + dx, y1);
        device.setPoint(p2, x2 + dx, y2);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d", x1 + dx, y1);
        TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d", x2 + dx, y2);

        // release first finger
        TestLog.reset();
        device.removePoint(p1);
        device.sync();
        TestLog.waitForLogContaining("Touch released: %d, %d", x1 + dx, y1);
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x1 + dx, y1);
        TestLog.waitForLogContaining("TouchPoint: STATIONARY %d, %d", x2 + dx, y2);
        // release second finger
        TestLog.reset();
        device.removePoint(p2);
        device.sync();
        TestLog.waitForLogContaining("Touch released: %d, %d", x2 + dx, y2);
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x2 + dx, y2);

        // tap
        int p3 = device.addPoint(x1, y1 + dy);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x1, y1 + dy);
        // release
        device.removePoint(p3);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x1, y1 + dy);
    }

    @Test
    public void twoFingersPressDragOne() throws Exception {
        int delta = device.getTapRadius() + 1;
        TestLog.reset();
        int x1 = (int) Math.round(width * 0.2);
        int y1 = (int) Math.round(height * 0.2);
        int x2 = (int) Math.round(width * 0.5);
        int y2 = (int) Math.round(height * 0.5);

        //press two fingers
        int p1 = device.addPoint(x1, y1);
        int p2 = device.addPoint(x2, y2);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x2, y2);

        //drag second finger only
        TestLog.reset();
        for (int i = 1; i < 4; i++) {
            x2 += delta;
            device.setPoint(p2, x2, y2);
            device.sync();
            TestLog.waitForLogContaining("TouchPoint: MOVED %d, %d", x2, y2);
        }

        TestLog.reset();
        device.removePoint(p1);
        device.removePoint(p2);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x1, y1);
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x2, y2);

        //press one finger
        TestLog.reset();
        int p3 = device.addPoint(x1, y1);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: PRESSED %d, %d", x1, y1);
        TestLog.waitForLogContaining("Mouse pressed: %d, %d", x1, y1);
        TestLog.waitForLogContaining("Touch points count: [1]");

        //release finger
        device.removePoint(p3);
        device.sync();
        TestLog.waitForLogContaining("TouchPoint: RELEASED %d, %d", x1, y1);
        TestLog.waitForLogContaining("Mouse released: %d, %d", x1, y1);
        TestLog.waitForLogContaining("Mouse clicked: %d, %d", x1, y1);
    }
}
