/*
 * Copyright (c) 2013 Oracle and/or its affiliates. All rights reserved.
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

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_oracle_dalvik_FXActivity_InternalSurfaceView */

#ifndef _Included_com_oracle_dalvik_FXActivity_InternalSurfaceView
#define _Included_com_oracle_dalvik_FXActivity_InternalSurfaceView
#ifdef __cplusplus
extern "C" {
#endif
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_NO_ID
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_NO_ID -1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_VISIBLE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_VISIBLE 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_INVISIBLE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_INVISIBLE 4L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_GONE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_GONE 8L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_DRAWING_CACHE_QUALITY_LOW
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_DRAWING_CACHE_QUALITY_LOW 524288L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_DRAWING_CACHE_QUALITY_HIGH
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_DRAWING_CACHE_QUALITY_HIGH 1048576L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_DRAWING_CACHE_QUALITY_AUTO
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_DRAWING_CACHE_QUALITY_AUTO 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBARS_INSIDE_OVERLAY
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBARS_INSIDE_OVERLAY 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBARS_INSIDE_INSET
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBARS_INSIDE_INSET 16777216L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBARS_OUTSIDE_OVERLAY
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBARS_OUTSIDE_OVERLAY 33554432L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBARS_OUTSIDE_INSET
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBARS_OUTSIDE_INSET 50331648L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_KEEP_SCREEN_ON
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_KEEP_SCREEN_ON 67108864L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SOUND_EFFECTS_ENABLED
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SOUND_EFFECTS_ENABLED 134217728L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_HAPTIC_FEEDBACK_ENABLED
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_HAPTIC_FEEDBACK_ENABLED 268435456L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUSABLES_ALL
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUSABLES_ALL 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUSABLES_TOUCH_MODE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUSABLES_TOUCH_MODE 1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_BACKWARD
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_BACKWARD 1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_FORWARD
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_FORWARD 2L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_LEFT
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_LEFT 17L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_UP
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_UP 33L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_RIGHT
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_RIGHT 66L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_DOWN
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_FOCUS_DOWN 130L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_MEASURED_SIZE_MASK
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_MEASURED_SIZE_MASK 16777215L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_MEASURED_STATE_MASK
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_MEASURED_STATE_MASK -16777216L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_MEASURED_HEIGHT_STATE_SHIFT
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_MEASURED_HEIGHT_STATE_SHIFT 16L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_MEASURED_STATE_TOO_SMALL
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_MEASURED_STATE_TOO_SMALL 16777216L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_TEXT_ALIGNMENT_INHERIT
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_TEXT_ALIGNMENT_INHERIT 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_TEXT_ALIGNMENT_RESOLVED_DEFAULT
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_TEXT_ALIGNMENT_RESOLVED_DEFAULT 131072L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_IMPORTANT_FOR_ACCESSIBILITY_AUTO
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_IMPORTANT_FOR_ACCESSIBILITY_AUTO 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_IMPORTANT_FOR_ACCESSIBILITY_YES
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_IMPORTANT_FOR_ACCESSIBILITY_YES 1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_IMPORTANT_FOR_ACCESSIBILITY_NO
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_IMPORTANT_FOR_ACCESSIBILITY_NO 2L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_OVER_SCROLL_ALWAYS
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_OVER_SCROLL_ALWAYS 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_OVER_SCROLL_IF_CONTENT_SCROLLS
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_OVER_SCROLL_IF_CONTENT_SCROLLS 1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_OVER_SCROLL_NEVER
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_OVER_SCROLL_NEVER 2L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_VISIBLE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_VISIBLE 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_LOW_PROFILE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_LOW_PROFILE 1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_HIDE_NAVIGATION
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_HIDE_NAVIGATION 2L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_FULLSCREEN
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_FULLSCREEN 4L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_LAYOUT_STABLE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_LAYOUT_STABLE 256L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION 512L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN 1024L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_STATUS_BAR_HIDDEN
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_STATUS_BAR_HIDDEN 1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_STATUS_BAR_VISIBLE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_STATUS_BAR_VISIBLE 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_LAYOUT_FLAGS
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SYSTEM_UI_LAYOUT_FLAGS 1536L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_FIND_VIEWS_WITH_TEXT
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_FIND_VIEWS_WITH_TEXT 1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_FIND_VIEWS_WITH_CONTENT_DESCRIPTION
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_FIND_VIEWS_WITH_CONTENT_DESCRIPTION 2L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SCREEN_STATE_OFF
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SCREEN_STATE_OFF 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SCREEN_STATE_ON
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SCREEN_STATE_ON 1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBAR_POSITION_DEFAULT
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBAR_POSITION_DEFAULT 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBAR_POSITION_LEFT
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBAR_POSITION_LEFT 1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBAR_POSITION_RIGHT
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_SCROLLBAR_POSITION_RIGHT 2L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_LAYER_TYPE_NONE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_LAYER_TYPE_NONE 0L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_LAYER_TYPE_SOFTWARE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_LAYER_TYPE_SOFTWARE 1L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_LAYER_TYPE_HARDWARE
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_LAYER_TYPE_HARDWARE 2L
#undef com_oracle_dalvik_FXActivity_InternalSurfaceView_ACTION_POINTER_STILL
#define com_oracle_dalvik_FXActivity_InternalSurfaceView_ACTION_POINTER_STILL -1L
/*
 * Class:     com_oracle_dalvik_FXActivity_InternalSurfaceView
 * Method:    onMultiTouchEventNative
 * Signature: (I[I[I[I[I)V
 */
JNIEXPORT void JNICALL Java_com_oracle_dalvik_FXActivity_00024InternalSurfaceView_onMultiTouchEventNative
  (JNIEnv *, jobject, jint, jintArray, jintArray, jintArray, jintArray);

/*
 * Class:     com_oracle_dalvik_FXActivity_InternalSurfaceView
 * Method:    onKeyEventNative
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_oracle_dalvik_FXActivity_00024InternalSurfaceView_onKeyEventNative
  (JNIEnv *, jobject, jint, jint, jstring);

#ifdef __cplusplus
}
#endif
#endif
