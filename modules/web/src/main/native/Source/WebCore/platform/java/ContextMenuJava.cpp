/*
 * Copyright (c) 2011, 2016, Oracle and/or its affiliates. All rights reserved.
 */
#include "config.h"

#include "ContextMenuJava.h"
#include "ContextMenu.h"
#include "ContextMenuController.h"
#include "ContextMenuItem.h"
#include <wtf/java/JavaEnv.h>

#include "com_sun_webkit_ContextMenu.h"
#include "com_sun_webkit_ContextMenuItem.h"


namespace WebCore {

static jclass getJContextMenuItemClass()
{
    JNIEnv* env = WebCore_GetJavaEnv();
    static JGClass jContextMenuItemClass = JLClass(env->FindClass("com/sun/webkit/ContextMenuItem"));
    ASSERT(jContextMenuItemClass);
    return (jclass)jContextMenuItemClass;
}

static JGObject createJavaMenuItem()
{
    JNIEnv* env = WebCore_GetJavaEnv();

    static jmethodID createContextMenuItemMID = env->GetStaticMethodID(getJContextMenuItemClass(), "fwkCreateContextMenuItem",
                                                      "()Lcom/sun/webkit/ContextMenuItem;");
    ASSERT(createContextMenuItemMID);

    JGObject jContextMenuItem(env->CallStaticObjectMethod(getJContextMenuItemClass(), createContextMenuItemMID));
    CheckAndClearException(env);
    return jContextMenuItem;
}

class ContextMenuItemJava {
  private:
    JGObject m_menuItem;
  public:
    ContextMenuItemJava()
      : m_menuItem(createJavaMenuItem()) {
    }

    void setType(ContextMenuItemType type)
    {
        if (!m_menuItem) {
            return;
        }
        JNIEnv* env = WebCore_GetJavaEnv();
        static jmethodID setTypeMID = env->GetMethodID(getJContextMenuItemClass(), "fwkSetType", "(I)V");
        ASSERT(setTypeMID);

        jint jtype = com_sun_webkit_ContextMenuItem_ACTION_TYPE;
        if (SeparatorType == type) {
            jtype = com_sun_webkit_ContextMenuItem_SEPARATOR_TYPE;
        } else if (SubmenuType == type) {
            jtype = com_sun_webkit_ContextMenuItem_SUBMENU_TYPE;
        }
        env->CallVoidMethod(m_menuItem, setTypeMID, jtype);
        CheckAndClearException(env);
    }

    void setAction(ContextMenuAction action)
    {
        if (!m_menuItem) {
            return;
        }
        JNIEnv* env = WebCore_GetJavaEnv();

        static jmethodID setActionMID = env->GetMethodID(getJContextMenuItemClass(), "fwkSetAction", "(I)V");
        ASSERT(setActionMID);
        env->CallVoidMethod(m_menuItem, setActionMID, action);
        CheckAndClearException(env);
    }

    void setTitle(const String& title)
    {
        if (!m_menuItem) {
            return;
        }
        JNIEnv* env = WebCore_GetJavaEnv();

        static jmethodID setTitleMID = env->GetMethodID(getJContextMenuItemClass(), "fwkSetTitle", "(Ljava/lang/String;)V");
        ASSERT(setTitleMID);

        env->CallVoidMethod(m_menuItem, setTitleMID, title.isEmpty() ? NULL : (jstring)title.toJavaString(env));
        CheckAndClearException(env);
    }

    void setSubMenu(JGObject obj)
    {
        if (!m_menuItem) {
            return;
        }
        JNIEnv* env = WebCore_GetJavaEnv();

        static jmethodID setSubmenuMID = env->GetMethodID(getJContextMenuItemClass(), "fwkSetSubmenu",
                                         "(Lcom/sun/webkit/ContextMenu;)V");
        ASSERT(setSubmenuMID);
        JLObject submenu(obj);
        env->CallVoidMethod(m_menuItem, setSubmenuMID, (jobject)submenu);
        CheckAndClearException(env);
    }

    void setChecked(bool checked)
    {
        if (!m_menuItem) {
            return;
        }
        JNIEnv* env = WebCore_GetJavaEnv();
        static jmethodID setCheckedMID = env->GetMethodID(getJContextMenuItemClass(), "fwkSetChecked", "(Z)V");
        ASSERT(setCheckedMID);

        env->CallVoidMethod(m_menuItem, setCheckedMID, bool_to_jbool(checked));
        CheckAndClearException(env);
    }

    void setEnabled(bool enabled)
    {
        if (!m_menuItem) {
            return;
        }
        JNIEnv* env = WebCore_GetJavaEnv();
        static jmethodID setEnabledMID = env->GetMethodID(getJContextMenuItemClass(), "fwkSetEnabled", "(Z)V");
        ASSERT(setEnabledMID);

        env->CallVoidMethod(m_menuItem, setEnabledMID, bool_to_jbool(enabled));
        CheckAndClearException(env);
    }

    operator jobject() const {
      return (jobject)m_menuItem;
    }
};

static jclass getJContextMenuClass()
{
    JNIEnv* env = WebCore_GetJavaEnv();
    static JGClass jContextMenuClass(env->FindClass("com/sun/webkit/ContextMenu"));
    ASSERT(jContextMenuClass);
    return (jclass)jContextMenuClass;
}

static JLObject createJavaContextMenu()
{
    JNIEnv* env = WebCore_GetJavaEnv();

    static jmethodID mid = env->GetStaticMethodID(getJContextMenuClass(),
            "fwkCreateContextMenu",
            "()Lcom/sun/webkit/ContextMenu;");
    ASSERT(mid);

    JLObject jContextMenu(env->CallStaticObjectMethod(getJContextMenuClass(), mid));
    ASSERT(jContextMenu);
    CheckAndClearException(env);

    return jContextMenu;
}

ContextMenuJava::ContextMenuJava(const Vector<ContextMenuItem>& items)
    : m_contextMenu(createJavaContextMenu())
{
    if (!m_contextMenu) {
        return;
    }

    JNIEnv* env = WebCore_GetJavaEnv();
    static jmethodID mid = env->GetMethodID(getJContextMenuClass(),
        "fwkAppendItem", "(Lcom/sun/webkit/ContextMenuItem;)V");
    ASSERT(mid);

    for (const auto& item : items) {
        if (item.isNull() ||
              (item.type() != SeparatorType && item.title().isEmpty())) {
            continue;
        }
        ContextMenuItemJava menuItem;
        menuItem.setType(item.type());
        menuItem.setAction(item.action());
        menuItem.setTitle(item.title());
        menuItem.setEnabled(item.enabled());
        menuItem.setChecked(item.checked());
        // Call recursively
        menuItem.setSubMenu(ContextMenuJava(item.subMenuItems()).m_contextMenu);
        env->CallVoidMethod(m_contextMenu, mid, (jobject)menuItem);
        CheckAndClearException(env);
    }
}

void ContextMenuJava::show(ContextMenuController* ctrl, jobject page, const IntPoint& loc) const
{
    if (!m_contextMenu) {
        return;
    }

    JNIEnv* env = WebCore_GetJavaEnv();
    static jmethodID mid = env->GetMethodID(
            getJContextMenuClass(),
            "fwkShow",
            "(Lcom/sun/webkit/WebPage;JII)V");
    ASSERT(mid);

    env->CallVoidMethod(
            m_contextMenu,
            mid,
            page,
            ptr_to_jlong(ctrl),
            loc.x(),
            loc.y());
    CheckAndClearException(env);
}

} // namespace WebCore

using namespace WebCore;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_sun_webkit_ContextMenu_twkHandleItemSelected
    (JNIEnv*, jobject, jlong menuCtrlPData, jint itemAction)
{
    ContextMenuController* cmc = static_cast<ContextMenuController*>jlong_to_ptr(menuCtrlPData);
    cmc->contextMenuItemSelected((ContextMenuAction)itemAction, "aux");
}

#ifdef __cplusplus
}
#endif
