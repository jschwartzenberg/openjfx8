/*
 * Copyright (c) 2009, 2013, Oracle and/or its affiliates. All rights reserved.
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

package com.sun.prism.es2;

import com.sun.prism.CompositeMode;
import com.sun.prism.RenderTarget;
import com.sun.prism.impl.ps.BaseShaderGraphics;
import com.sun.prism.paint.Color;
import com.sun.prism.paint.Paint;

public class ES2Graphics extends BaseShaderGraphics {

    private final ES2Context context;

    private ES2Graphics(ES2Context context, RenderTarget target) {
        super(context, target);
        this.context = context;
    }

    static ES2Graphics create(ES2Context context, RenderTarget target) {
        if (target == null) {
            return null;
        }
        return new ES2Graphics(context, target);
    }

    static void clearBuffers(ES2Context context, Color color, boolean clearColor,
            boolean clearDepth, boolean ignoreScissor) {
        context.getGLContext().clearBuffers(color, clearColor, clearDepth,
                ignoreScissor);

    }

    public void clearQuad(float x1, float y1, float x2, float y2) {
        // note that unlike clear(), this method does not currently
        // attempt to clear the depth buffer...
        context.setRenderTarget(this);
        context.flushVertexBuffer();
        CompositeMode mode = getCompositeMode();
        // set the blend mode to CLEAR
        context.updateCompositeMode(CompositeMode.CLEAR);
        Paint oldPaint = getPaint();
        setPaint(Color.BLACK); // any color will do...
        fillQuad(x1, y1, x2, y2);
        context.flushVertexBuffer();
        setPaint(oldPaint);
        // restore default blend mode
        context.updateCompositeMode(mode);
    }

    public void clear(Color color) {
        context.validateClearOp(this);
        this.getRenderTarget().setOpaque(color.isOpaque());
        clearBuffers(context, color, true, isDepthBuffer(), false);

    }

    public void sync() {
        context.flushVertexBuffer();
        context.getGLContext().finish();
    }

    /**
     * Called from ES2SwapChain to force the render target to be revalidated
     * (context made current, viewport and projection matrix updated, etc)
     * in response to a window resize event.
     */
    void forceRenderTarget() {
        context.forceRenderTarget(this);
    }

}
