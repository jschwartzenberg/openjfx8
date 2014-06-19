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

package javafx.scene.transform;

/**
Builder class for javafx.scene.transform.Affine
@see javafx.scene.transform.Affine
@deprecated This class is deprecated and will be removed in the next version
* @since JavaFX 2.0
*/
@javax.annotation.Generated("Generated by javafx.builder.processor.BuilderProcessor")
@Deprecated
public class AffineBuilder<B extends javafx.scene.transform.AffineBuilder<B>> implements javafx.util.Builder<javafx.scene.transform.Affine> {
    protected AffineBuilder() {
    }
    
    /** Creates a new instance of AffineBuilder. */
    @SuppressWarnings({"deprecation", "rawtypes", "unchecked"})
    public static javafx.scene.transform.AffineBuilder<?> create() {
        return new javafx.scene.transform.AffineBuilder();
    }
    
    private int __set;
    private void __set(int i) {
        __set |= 1 << i;
    }
    public void applyTo(javafx.scene.transform.Affine x) {
        int set = __set;
        while (set != 0) {
            int i = Integer.numberOfTrailingZeros(set);
            set &= ~(1 << i);
            switch (i) {
                case 0: x.setMxx(this.mxx); break;
                case 1: x.setMxy(this.mxy); break;
                case 2: x.setMxz(this.mxz); break;
                case 3: x.setMyx(this.myx); break;
                case 4: x.setMyy(this.myy); break;
                case 5: x.setMyz(this.myz); break;
                case 6: x.setMzx(this.mzx); break;
                case 7: x.setMzy(this.mzy); break;
                case 8: x.setMzz(this.mzz); break;
                case 9: x.setTx(this.tx); break;
                case 10: x.setTy(this.ty); break;
                case 11: x.setTz(this.tz); break;
            }
        }
    }
    
    private double mxx;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getMxx() mxx} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B mxx(double x) {
        this.mxx = x;
        __set(0);
        return (B) this;
    }
    
    private double mxy;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getMxy() mxy} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B mxy(double x) {
        this.mxy = x;
        __set(1);
        return (B) this;
    }
    
    private double mxz;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getMxz() mxz} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B mxz(double x) {
        this.mxz = x;
        __set(2);
        return (B) this;
    }
    
    private double myx;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getMyx() myx} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B myx(double x) {
        this.myx = x;
        __set(3);
        return (B) this;
    }
    
    private double myy;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getMyy() myy} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B myy(double x) {
        this.myy = x;
        __set(4);
        return (B) this;
    }
    
    private double myz;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getMyz() myz} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B myz(double x) {
        this.myz = x;
        __set(5);
        return (B) this;
    }
    
    private double mzx;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getMzx() mzx} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B mzx(double x) {
        this.mzx = x;
        __set(6);
        return (B) this;
    }
    
    private double mzy;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getMzy() mzy} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B mzy(double x) {
        this.mzy = x;
        __set(7);
        return (B) this;
    }
    
    private double mzz;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getMzz() mzz} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B mzz(double x) {
        this.mzz = x;
        __set(8);
        return (B) this;
    }
    
    private double tx;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getTx() tx} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B tx(double x) {
        this.tx = x;
        __set(9);
        return (B) this;
    }
    
    private double ty;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getTy() ty} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B ty(double x) {
        this.ty = x;
        __set(10);
        return (B) this;
    }
    
    private double tz;
    /**
    Set the value of the {@link javafx.scene.transform.Affine#getTz() tz} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B tz(double x) {
        this.tz = x;
        __set(11);
        return (B) this;
    }
    
    /**
    Make an instance of {@link javafx.scene.transform.Affine} based on the properties set on this builder.
    */
    public javafx.scene.transform.Affine build() {
        javafx.scene.transform.Affine x = new javafx.scene.transform.Affine();
        applyTo(x);
        return x;
    }
}
