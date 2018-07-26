/*
 * Copyright (c) 2011, 2017, Oracle and/or its affiliates. All rights reserved.
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

#include "config.h"

#include "NotImplemented.h"

#include "Color.h"
#include "Frame.h"
#include "FrameView.h"
#include <wtf/java/JavaEnv.h>
#include "Page.h"
#include "PopupMenuJava.h"
#include "PopupMenuClient.h"
#include "RenderStyle.h"
#include "Font.h"
#include "WebPage.h"

#include <wtf/text/WTFString.h>

#include "com_sun_webkit_PopupMenu.h"

static jclass getJPopupMenuClass()
{
    JNIEnv* env = WebCore_GetJavaEnv();
    static JGClass jPopupMenuClass(env->FindClass("com/sun/webkit/PopupMenu"));
    ASSERT(jPopupMenuClass);
    return (jclass)jPopupMenuClass;
}

static void setSelectedItem(jobject popup, jint index)
{
    JNIEnv* env = WebCore_GetJavaEnv();
    static jmethodID setSelectedItemMID = env->GetMethodID(
        getJPopupMenuClass(), "fwkSetSelectedItem", "(I)V");
    ASSERT(setSelectedItemMID);

    env->CallVoidMethod(popup, setSelectedItemMID, index);
    CheckAndClearException(env);
}

namespace WebCore {

PopupMenuJava::PopupMenuJava(PopupMenuClient* client)
    : m_popupClient(client),
      m_popup(0)
{
}

PopupMenuJava::~PopupMenuJava()
{
    if (!m_popup)
        return;

    WC_GETJAVAENV_CHKRET(env);

    static jmethodID mid = env->GetMethodID(getJPopupMenuClass(),
        "fwkDestroy", "()V");
    ASSERT(mid);

    env->CallVoidMethod(m_popup, mid);
    CheckAndClearException(env);
}

void PopupMenuJava::createPopupMenuJava(Page*)
{
    JNIEnv* env = WebCore_GetJavaEnv();

    static jmethodID mid = env->GetStaticMethodID(getJPopupMenuClass(),
        "fwkCreatePopupMenu", "(J)Lcom/sun/webkit/PopupMenu;");
    ASSERT(mid);

    JLObject jPopupMenu(env->CallStaticObjectMethod(getJPopupMenuClass(), mid, ptr_to_jlong(this)));
    ASSERT(jPopupMenu);
    CheckAndClearException(env);

    m_popup = jPopupMenu;
}

void PopupMenuJava::populate()
{
    JNIEnv* env = WebCore_GetJavaEnv();

    static jmethodID mid = env->GetMethodID(getJPopupMenuClass(),
        "fwkAppendItem", "(Ljava/lang/String;ZZZIILcom/sun/webkit/graphics/WCFont;)V");
    ASSERT(mid);

    for (int i = 0; i < client()->listSize(); i++) {
        String itemText = client()->itemText(i);
        JLString itemTextJ(itemText.toJavaString(env));
        ASSERT(itemTextJ);
        PopupMenuStyle style = client()->itemStyle(i);
        env->CallVoidMethod(m_popup, mid, (jstring)itemTextJ,
                            bool_to_jbool(client()->itemIsLabel(i)),
                            bool_to_jbool(client()->itemIsSeparator(i)),
                            bool_to_jbool(client()->itemIsEnabled(i)),
                            style.backgroundColor().rgb(),
                            style.foregroundColor().rgb(),
                            (jobject)*style.font().primaryFont().platformData().nativeFontData());
        CheckAndClearException(env);
    }
}

void PopupMenuJava::show(const IntRect& r, FrameView* frameView, int index)
{
    JNIEnv* env = WebCore_GetJavaEnv();

    ASSERT(frameView->frame().page());

    createPopupMenuJava(frameView->frame().page());
    populate();
    setSelectedItem(m_popup, index);

    // r is in contents coordinates, while popup menu expects window coordinates
    IntRect wr = frameView->contentsToWindow(r);

    static jmethodID mid = env->GetMethodID(
            getJPopupMenuClass(),
            "fwkShow",
            "(Lcom/sun/webkit/WebPage;III)V");
    ASSERT(mid);

    env->CallVoidMethod(
            m_popup,
            mid,
            (jobject) WebPage::jobjectFromPage(frameView->frame().page()),
            wr.x(),
            wr.y() + wr.height(),
            wr.width());
    CheckAndClearException(env);
}

void PopupMenuJava::hide()
{
    JNIEnv* env = WebCore_GetJavaEnv();

    static jmethodID mid = env->GetMethodID(getJPopupMenuClass(), "fwkHide", "()V");
    ASSERT(mid);

    env->CallVoidMethod((jobject)m_popup, mid);
    CheckAndClearException(env);
}

void PopupMenuJava::updateFromElement()
{
    client()->setTextFromItem(client()->selectedIndex());
    if (!m_popup) {
        return;
    }
    setSelectedItem(m_popup, client()->selectedIndex());
}

void PopupMenuJava::disconnectClient()
{
    m_popupClient = 0;
}

} // namespace WebCore

using namespace WebCore;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_sun_webkit_PopupMenu_twkSelectionCommited
    (JNIEnv*, jobject, jlong pdata, jint index)
{
    if (!pdata) {
        return;
    }

    PopupMenuJava* pPopupMenu = static_cast<PopupMenuJava*>(jlong_to_ptr(pdata));
    ASSERT(pPopupMenu);

    if (pPopupMenu->client()) {
        pPopupMenu->client()->valueChanged(index);
    }
}

JNIEXPORT void JNICALL Java_com_sun_webkit_PopupMenu_twkPopupClosed
    (JNIEnv*, jobject, jlong pdata)
{
    if (!pdata) {
        return;
    }

    PopupMenuJava* pPopupMenu = static_cast<PopupMenuJava*>(jlong_to_ptr(pdata));
    ASSERT(pPopupMenu);

    if (pPopupMenu->client()) {
        pPopupMenu->client()->popupDidHide();
    }
}

#ifdef __cplusplus
}
#endif
