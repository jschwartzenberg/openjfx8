/*
 *  This file is part of the WebKit open source project.
 *  This file has been generated by generate-bindings.pl. DO NOT MODIFY!
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "WebKitDOMTestInterface.h"

#include "CSSImportRule.h"
#include "DOMObjectCache.h"
#include "Document.h"
#include "ExceptionCode.h"
#include "JSMainThreadExecState.h"
#include "TestSupplemental.h"
#include "WebKitDOMNodePrivate.h"
#include "WebKitDOMPrivate.h"
#include "WebKitDOMTestInterfacePrivate.h"
#include "WebKitDOMTestObjPrivate.h"
#include "gobject/ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_TEST_INTERFACE_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_TYPE_DOM_TEST_INTERFACE, WebKitDOMTestInterfacePrivate)

typedef struct _WebKitDOMTestInterfacePrivate {
#if ENABLE(Condition1) || ENABLE(Condition2)
    RefPtr<WebCore::TestInterface> coreObject;
#endif // ENABLE(Condition1) || ENABLE(Condition2)
} WebKitDOMTestInterfacePrivate;

#if ENABLE(Condition1) || ENABLE(Condition2)

namespace WebKit {

WebKitDOMTestInterface* kit(WebCore::TestInterface* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_TEST_INTERFACE(ret);

    return wrapTestInterface(obj);
}

WebCore::TestInterface* core(WebKitDOMTestInterface* request)
{
    return request ? static_cast<WebCore::TestInterface*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

WebKitDOMTestInterface* wrapTestInterface(WebCore::TestInterface* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_TEST_INTERFACE(g_object_new(WEBKIT_TYPE_DOM_TEST_INTERFACE, "core-object", coreObject, NULL));
}

} // namespace WebKit

#endif // ENABLE(Condition1) || ENABLE(Condition2)

G_DEFINE_TYPE(WebKitDOMTestInterface, webkit_dom_test_interface, WEBKIT_TYPE_DOM_OBJECT)

enum {
    PROP_0,
    PROP_IMPLEMENTS_STR1,
    PROP_IMPLEMENTS_STR2,
    PROP_IMPLEMENTS_NODE,
    PROP_SUPPLEMENTAL_STR1,
    PROP_SUPPLEMENTAL_STR2,
    PROP_SUPPLEMENTAL_NODE,
};

static void webkit_dom_test_interface_finalize(GObject* object)
{
    WebKitDOMTestInterfacePrivate* priv = WEBKIT_DOM_TEST_INTERFACE_GET_PRIVATE(object);
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebKit::DOMObjectCache::forget(priv->coreObject.get());
#endif // ENABLE(Condition1) || ENABLE(Condition2)
    priv->~WebKitDOMTestInterfacePrivate();
    G_OBJECT_CLASS(webkit_dom_test_interface_parent_class)->finalize(object);
}

static void webkit_dom_test_interface_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebKitDOMTestInterface* self = WEBKIT_DOM_TEST_INTERFACE(object);
    WebCore::TestInterface* coreSelf = WebKit::core(self);
#endif // ENABLE(Condition1) || ENABLE(Condition2)
    switch (propertyId) {
    case PROP_IMPLEMENTS_STR2: {
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
        coreSelf->setImplementsStr2(WTF::String::fromUTF8(g_value_get_string(value)));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    case PROP_SUPPLEMENTAL_STR2: {
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
        WebCore::TestSupplemental::setSupplementalStr2(coreSelf, WTF::String::fromUTF8(g_value_get_string(value)));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_test_interface_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebKitDOMTestInterface* self = WEBKIT_DOM_TEST_INTERFACE(object);
    WebCore::TestInterface* coreSelf = WebKit::core(self);
#endif // ENABLE(Condition1) || ENABLE(Condition2)
    switch (propertyId) {
    case PROP_IMPLEMENTS_STR1: {
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
        g_value_take_string(value, convertToUTF8String(coreSelf->implementsStr1()));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    case PROP_IMPLEMENTS_STR2: {
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
        g_value_take_string(value, convertToUTF8String(coreSelf->implementsStr2()));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    case PROP_IMPLEMENTS_NODE: {
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
        RefPtr<WebCore::Node> ptr = coreSelf->implementsNode();
        g_value_set_object(value, WebKit::kit(ptr.get()));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    case PROP_SUPPLEMENTAL_STR1: {
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
        g_value_take_string(value, convertToUTF8String(WebCore::TestSupplemental::supplementalStr1(coreSelf)));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    case PROP_SUPPLEMENTAL_STR2: {
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
        g_value_take_string(value, convertToUTF8String(WebCore::TestSupplemental::supplementalStr2(coreSelf)));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    case PROP_SUPPLEMENTAL_NODE: {
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
        RefPtr<WebCore::Node> ptr = WebCore::TestSupplemental::supplementalNode(coreSelf);
        g_value_set_object(value, WebKit::kit(ptr.get()));
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
        WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
        break;
    }
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_test_interface_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_test_interface_parent_class)->constructor(type, constructPropertiesCount, constructProperties);
#if ENABLE(Condition1) || ENABLE(Condition2)
    WebKitDOMTestInterfacePrivate* priv = WEBKIT_DOM_TEST_INTERFACE_GET_PRIVATE(object);
    priv->coreObject = static_cast<WebCore::TestInterface*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    WebKit::DOMObjectCache::put(priv->coreObject.get(), object);
#endif // ENABLE(Condition1) || ENABLE(Condition2)
    return object;
}

static void webkit_dom_test_interface_class_init(WebKitDOMTestInterfaceClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(WebKitDOMTestInterfacePrivate));
    gobjectClass->constructor = webkit_dom_test_interface_constructor;
    gobjectClass->finalize = webkit_dom_test_interface_finalize;
    gobjectClass->set_property = webkit_dom_test_interface_set_property;
    gobjectClass->get_property = webkit_dom_test_interface_get_property;

    g_object_class_install_property(
        gobjectClass,
        PROP_IMPLEMENTS_STR1,
        g_param_spec_string(
            "implements-str1",
            "TestInterface:implements-str1",
            "read-only gchar* TestInterface:implements-str1",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        PROP_IMPLEMENTS_STR2,
        g_param_spec_string(
            "implements-str2",
            "TestInterface:implements-str2",
            "read-only gchar* TestInterface:implements-str2",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        PROP_IMPLEMENTS_NODE,
        g_param_spec_object(
            "implements-node",
            "TestInterface:implements-node",
            "read-only WebKitDOMNode* TestInterface:implements-node",
            WEBKIT_TYPE_DOM_NODE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        PROP_SUPPLEMENTAL_STR1,
        g_param_spec_string(
            "supplemental-str1",
            "TestInterface:supplemental-str1",
            "read-only gchar* TestInterface:supplemental-str1",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        PROP_SUPPLEMENTAL_STR2,
        g_param_spec_string(
            "supplemental-str2",
            "TestInterface:supplemental-str2",
            "read-only gchar* TestInterface:supplemental-str2",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        PROP_SUPPLEMENTAL_NODE,
        g_param_spec_object(
            "supplemental-node",
            "TestInterface:supplemental-node",
            "read-only WebKitDOMNode* TestInterface:supplemental-node",
            WEBKIT_TYPE_DOM_NODE,
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_test_interface_init(WebKitDOMTestInterface* request)
{
    WebKitDOMTestInterfacePrivate* priv = WEBKIT_DOM_TEST_INTERFACE_GET_PRIVATE(request);
    new (priv) WebKitDOMTestInterfacePrivate();
}

void webkit_dom_test_interface_implements_method1(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self));
    WebCore::TestInterface* item = WebKit::core(self);
    item->implementsMethod1();
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMTestObj* webkit_dom_test_interface_implements_method2(WebKitDOMTestInterface* self, const gchar* strArg, WebKitDOMTestObj* objArg, GError** error)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self), 0);
    g_return_val_if_fail(strArg, 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_OBJ(objArg), 0);
    g_return_val_if_fail(!error || !*error, 0);
    WebCore::TestInterface* item = WebKit::core(self);
    WTF::String convertedStrArg = WTF::String::fromUTF8(strArg);
    WebCore::TestObj* convertedObjArg = WebKit::core(objArg);
    WebCore::ExceptionCode ec = 0;
    RefPtr<WebCore::TestObj> gobjectResult = WTF::getPtr(item->implementsMethod2(convertedStrArg, convertedObjArg, ec));
    if (ec) {
        WebCore::ExceptionCodeDescription ecdesc(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), ecdesc.code, ecdesc.name);
    }
    return WebKit::kit(gobjectResult.get());
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
    return 0;
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void webkit_dom_test_interface_implements_method4(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self));
    WebCore::TestInterface* item = WebKit::core(self);
    item->implementsMethod4();
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void webkit_dom_test_interface_supplemental_method1(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self));
    WebCore::TestInterface* item = WebKit::core(self);
    WebCore::TestSupplemental::supplementalMethod1(item);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMTestObj* webkit_dom_test_interface_supplemental_method2(WebKitDOMTestInterface* self, const gchar* strArg, WebKitDOMTestObj* objArg, GError** error)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self), 0);
    g_return_val_if_fail(strArg, 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_OBJ(objArg), 0);
    g_return_val_if_fail(!error || !*error, 0);
    WebCore::TestInterface* item = WebKit::core(self);
    WTF::String convertedStrArg = WTF::String::fromUTF8(strArg);
    WebCore::TestObj* convertedObjArg = WebKit::core(objArg);
    WebCore::ExceptionCode ec = 0;
    RefPtr<WebCore::TestObj> gobjectResult = WTF::getPtr(WebCore::TestSupplemental::supplementalMethod2(item, convertedStrArg, convertedObjArg, ec));
    if (ec) {
        WebCore::ExceptionCodeDescription ecdesc(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), ecdesc.code, ecdesc.name);
    }
    return WebKit::kit(gobjectResult.get());
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
    return 0;
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void webkit_dom_test_interface_supplemental_method4(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self));
    WebCore::TestInterface* item = WebKit::core(self);
    WebCore::TestSupplemental::supplementalMethod4(item);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

gchar* webkit_dom_test_interface_get_implements_str1(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self), 0);
    WebCore::TestInterface* item = WebKit::core(self);
    gchar* result = convertToUTF8String(item->implementsStr1());
    return result;
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
    return 0;
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

gchar* webkit_dom_test_interface_get_implements_str2(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self), 0);
    WebCore::TestInterface* item = WebKit::core(self);
    gchar* result = convertToUTF8String(item->implementsStr2());
    return result;
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
    return 0;
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void webkit_dom_test_interface_set_implements_str2(WebKitDOMTestInterface* self, const gchar* value)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self));
    g_return_if_fail(value);
    WebCore::TestInterface* item = WebKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setImplementsStr2(convertedValue);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMNode* webkit_dom_test_interface_get_implements_node(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self), 0);
    WebCore::TestInterface* item = WebKit::core(self);
    RefPtr<WebCore::Node> gobjectResult = WTF::getPtr(item->implementsNode());
    return WebKit::kit(gobjectResult.get());
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
    return 0;
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void webkit_dom_test_interface_set_implements_node(WebKitDOMTestInterface* self, WebKitDOMNode* value)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition22) || ENABLE(Condition23)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self));
    g_return_if_fail(WEBKIT_DOM_IS_NODE(value));
    WebCore::TestInterface* item = WebKit::core(self);
    WebCore::Node* convertedValue = WebKit::core(value);
    item->setImplementsNode(convertedValue);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition22")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition23")
#endif /* ENABLE(Condition22) || ENABLE(Condition23) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

gchar* webkit_dom_test_interface_get_supplemental_str1(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self), 0);
    WebCore::TestInterface* item = WebKit::core(self);
    gchar* result = convertToUTF8String(WebCore::TestSupplemental::supplementalStr1(item));
    return result;
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
    return 0;
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

gchar* webkit_dom_test_interface_get_supplemental_str2(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self), 0);
    WebCore::TestInterface* item = WebKit::core(self);
    gchar* result = convertToUTF8String(WebCore::TestSupplemental::supplementalStr2(item));
    return result;
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
    return 0;
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void webkit_dom_test_interface_set_supplemental_str2(WebKitDOMTestInterface* self, const gchar* value)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self));
    g_return_if_fail(value);
    WebCore::TestInterface* item = WebKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    WebCore::TestSupplemental::setSupplementalStr2(item, convertedValue);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

WebKitDOMNode* webkit_dom_test_interface_get_supplemental_node(WebKitDOMTestInterface* self)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self), 0);
    WebCore::TestInterface* item = WebKit::core(self);
    RefPtr<WebCore::Node> gobjectResult = WTF::getPtr(WebCore::TestSupplemental::supplementalNode(item));
    return WebKit::kit(gobjectResult.get());
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
    return 0;
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
    return 0;
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

void webkit_dom_test_interface_set_supplemental_node(WebKitDOMTestInterface* self, WebKitDOMNode* value)
{
#if ENABLE(Condition1) || ENABLE(Condition2)
#if ENABLE(Condition11) || ENABLE(Condition12)
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_INTERFACE(self));
    g_return_if_fail(WEBKIT_DOM_IS_NODE(value));
    WebCore::TestInterface* item = WebKit::core(self);
    WebCore::Node* convertedValue = WebKit::core(value);
    WebCore::TestSupplemental::setSupplementalNode(item, convertedValue);
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition11")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition12")
#endif /* ENABLE(Condition11) || ENABLE(Condition12) */
#else
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition1")
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Condition2")
#endif /* ENABLE(Condition1) || ENABLE(Condition2) */
}

