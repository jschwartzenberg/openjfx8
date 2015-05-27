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
#include "WebKitDOMTestTypedefs.h"

#include "CSSImportRule.h"
#include "DOMObjectCache.h"
#include "Document.h"
#include "ExceptionCode.h"
#include "JSMainThreadExecState.h"
#include "WebKitDOMDOMString[]Private.h"
#include "WebKitDOMPrivate.h"
#include "WebKitDOMSVGPointPrivate.h"
#include "WebKitDOMSerializedScriptValuePrivate.h"
#include "WebKitDOMTestTypedefsPrivate.h"
#include "WebKitDOMlong[]Private.h"
#include "gobject/ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_TEST_TYPEDEFS_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_TYPE_DOM_TEST_TYPEDEFS, WebKitDOMTestTypedefsPrivate)

typedef struct _WebKitDOMTestTypedefsPrivate {
    RefPtr<WebCore::TestTypedefs> coreObject;
} WebKitDOMTestTypedefsPrivate;

namespace WebKit {

WebKitDOMTestTypedefs* kit(WebCore::TestTypedefs* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_TEST_TYPEDEFS(ret);

    return wrapTestTypedefs(obj);
}

WebCore::TestTypedefs* core(WebKitDOMTestTypedefs* request)
{
    return request ? static_cast<WebCore::TestTypedefs*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

WebKitDOMTestTypedefs* wrapTestTypedefs(WebCore::TestTypedefs* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_TEST_TYPEDEFS(g_object_new(WEBKIT_TYPE_DOM_TEST_TYPEDEFS, "core-object", coreObject, NULL));
}

} // namespace WebKit

G_DEFINE_TYPE(WebKitDOMTestTypedefs, webkit_dom_test_typedefs, WEBKIT_TYPE_DOM_OBJECT)

enum {
    PROP_0,
    PROP_UNSIGNED_LONG_LONG_ATTR,
    PROP_IMMUTABLE_SERIALIZED_SCRIPT_VALUE,
    PROP_ATTR_WITH_GETTER_EXCEPTION,
    PROP_ATTR_WITH_SETTER_EXCEPTION,
    PROP_STRING_ATTR_WITH_GETTER_EXCEPTION,
    PROP_STRING_ATTR_WITH_SETTER_EXCEPTION,
};

static void webkit_dom_test_typedefs_finalize(GObject* object)
{
    WebKitDOMTestTypedefsPrivate* priv = WEBKIT_DOM_TEST_TYPEDEFS_GET_PRIVATE(object);

    WebKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~WebKitDOMTestTypedefsPrivate();
    G_OBJECT_CLASS(webkit_dom_test_typedefs_parent_class)->finalize(object);
}

static void webkit_dom_test_typedefs_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;

    WebKitDOMTestTypedefs* self = WEBKIT_DOM_TEST_TYPEDEFS(object);
    WebCore::TestTypedefs* coreSelf = WebKit::core(self);

    switch (propertyId) {
    case PROP_UNSIGNED_LONG_LONG_ATTR: {
        coreSelf->setUnsignedLongLongAttr((g_value_get_uint64(value)));
        break;
    }
    case PROP_ATTR_WITH_GETTER_EXCEPTION: {
        coreSelf->setAttrWithGetterException((g_value_get_long(value)));
        break;
    }
    case PROP_ATTR_WITH_SETTER_EXCEPTION: {
        WebCore::ExceptionCode ec = 0;
        coreSelf->setAttrWithSetterException((g_value_get_long(value)), ec);
        break;
    }
    case PROP_STRING_ATTR_WITH_GETTER_EXCEPTION: {
        coreSelf->setStringAttrWithGetterException(WTF::String::fromUTF8(g_value_get_string(value)));
        break;
    }
    case PROP_STRING_ATTR_WITH_SETTER_EXCEPTION: {
        WebCore::ExceptionCode ec = 0;
        coreSelf->setStringAttrWithSetterException(WTF::String::fromUTF8(g_value_get_string(value)), ec);
        break;
    }
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_test_typedefs_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;

    WebKitDOMTestTypedefs* self = WEBKIT_DOM_TEST_TYPEDEFS(object);
    WebCore::TestTypedefs* coreSelf = WebKit::core(self);

    switch (propertyId) {
    case PROP_UNSIGNED_LONG_LONG_ATTR: {
        g_value_set_uint64(value, coreSelf->unsignedLongLongAttr());
        break;
    }
    case PROP_IMMUTABLE_SERIALIZED_SCRIPT_VALUE: {
        RefPtr<WebCore::SerializedScriptValue> ptr = coreSelf->immutableSerializedScriptValue();
        g_value_set_object(value, WebKit::kit(ptr.get()));
        break;
    }
    case PROP_ATTR_WITH_GETTER_EXCEPTION: {
        WebCore::ExceptionCode ec = 0;
        g_value_set_long(value, coreSelf->attrWithGetterException(ec));
        break;
    }
    case PROP_ATTR_WITH_SETTER_EXCEPTION: {
        g_value_set_long(value, coreSelf->attrWithSetterException());
        break;
    }
    case PROP_STRING_ATTR_WITH_GETTER_EXCEPTION: {
        WebCore::ExceptionCode ec = 0;
        g_value_take_string(value, convertToUTF8String(coreSelf->stringAttrWithGetterException(ec)));
        break;
    }
    case PROP_STRING_ATTR_WITH_SETTER_EXCEPTION: {
        g_value_take_string(value, convertToUTF8String(coreSelf->stringAttrWithSetterException()));
        break;
    }
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_test_typedefs_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_test_typedefs_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    WebKitDOMTestTypedefsPrivate* priv = WEBKIT_DOM_TEST_TYPEDEFS_GET_PRIVATE(object);
    priv->coreObject = static_cast<WebCore::TestTypedefs*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    WebKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_test_typedefs_class_init(WebKitDOMTestTypedefsClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(WebKitDOMTestTypedefsPrivate));
    gobjectClass->constructor = webkit_dom_test_typedefs_constructor;
    gobjectClass->finalize = webkit_dom_test_typedefs_finalize;
    gobjectClass->set_property = webkit_dom_test_typedefs_set_property;
    gobjectClass->get_property = webkit_dom_test_typedefs_get_property;

    g_object_class_install_property(
        gobjectClass,
        PROP_UNSIGNED_LONG_LONG_ATTR,
        g_param_spec_uint64(
            "unsigned-long-long-attr",
            "TestTypedefs:unsigned-long-long-attr",
            "read-only guint64 TestTypedefs:unsigned-long-long-attr",
            0, G_MAXUINT64, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        PROP_IMMUTABLE_SERIALIZED_SCRIPT_VALUE,
        g_param_spec_object(
            "immutable-serialized-script-value",
            "TestTypedefs:immutable-serialized-script-value",
            "read-only WebKitDOMSerializedScriptValue* TestTypedefs:immutable-serialized-script-value",
            WEBKIT_TYPE_DOM_SERIALIZED_SCRIPT_VALUE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        PROP_ATTR_WITH_GETTER_EXCEPTION,
        g_param_spec_long(
            "attr-with-getter-exception",
            "TestTypedefs:attr-with-getter-exception",
            "read-only glong TestTypedefs:attr-with-getter-exception",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        PROP_ATTR_WITH_SETTER_EXCEPTION,
        g_param_spec_long(
            "attr-with-setter-exception",
            "TestTypedefs:attr-with-setter-exception",
            "read-only glong TestTypedefs:attr-with-setter-exception",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        PROP_STRING_ATTR_WITH_GETTER_EXCEPTION,
        g_param_spec_string(
            "string-attr-with-getter-exception",
            "TestTypedefs:string-attr-with-getter-exception",
            "read-only gchar* TestTypedefs:string-attr-with-getter-exception",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        PROP_STRING_ATTR_WITH_SETTER_EXCEPTION,
        g_param_spec_string(
            "string-attr-with-setter-exception",
            "TestTypedefs:string-attr-with-setter-exception",
            "read-only gchar* TestTypedefs:string-attr-with-setter-exception",
            "",
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_test_typedefs_init(WebKitDOMTestTypedefs* request)
{
    WebKitDOMTestTypedefsPrivate* priv = WEBKIT_DOM_TEST_TYPEDEFS_GET_PRIVATE(request);
    new (priv) WebKitDOMTestTypedefsPrivate();
}

void webkit_dom_test_typedefs_func(WebKitDOMTestTypedefs* self, WebKitDOMlong[]* x)
{
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self));
    g_return_if_fail(WEBKIT_DOM_IS_LONG[](x));
    WebCore::TestTypedefs* item = WebKit::core(self);
    WebCore::long[]* convertedX = WebKit::core(x);
    item->func(convertedX);
}

void webkit_dom_test_typedefs_set_shadow(WebKitDOMTestTypedefs* self, gfloat width, gfloat height, gfloat blur, const gchar* color, gfloat alpha)
{
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self));
    g_return_if_fail(color);
    WebCore::TestTypedefs* item = WebKit::core(self);
    WTF::String convertedColor = WTF::String::fromUTF8(color);
    item->setShadow(width, height, blur, convertedColor, alpha);
}

void webkit_dom_test_typedefs_nullable_array_arg(WebKitDOMTestTypedefs* self, WebKitDOMDOMString[]* arrayArg)
{
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self));
    g_return_if_fail(WEBKIT_DOM_IS_DOM_STRING[](arrayArg));
    WebCore::TestTypedefs* item = WebKit::core(self);
    WebCore::DOMString[]* convertedArrayArg = WebKit::core(arrayArg);
    item->nullableArrayArg(convertedArrayArg);
}

WebKitDOMSVGPoint* webkit_dom_test_typedefs_immutable_point_function(WebKitDOMTestTypedefs* self)
{
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self), 0);
    WebCore::TestTypedefs* item = WebKit::core(self);
    RefPtr<WebCore::SVGPoint> gobjectResult = WTF::getPtr(item->immutablePointFunction());
    return WebKit::kit(gobjectResult.get());
}

WebKitDOMDOMString[]* webkit_dom_test_typedefs_string_array_function(WebKitDOMTestTypedefs* self, WebKitDOMDOMString[]* values, GError** error)
{
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_STRING[](values), 0);
    g_return_val_if_fail(!error || !*error, 0);
    WebCore::TestTypedefs* item = WebKit::core(self);
    WebCore::DOMString[]* convertedValues = WebKit::core(values);
    WebCore::ExceptionCode ec = 0;
    RefPtr<WebCore::DOMString[]> gobjectResult = WTF::getPtr(item->stringArrayFunction(convertedValues, ec));
    if (ec) {
        WebCore::ExceptionCodeDescription ecdesc(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), ecdesc.code, ecdesc.name);
    }
    return WebKit::kit(gobjectResult.get());
}

WebKitDOMDOMString[]* webkit_dom_test_typedefs_string_array_function2(WebKitDOMTestTypedefs* self, WebKitDOMDOMString[]* values, GError** error)
{
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_STRING[](values), 0);
    g_return_val_if_fail(!error || !*error, 0);
    WebCore::TestTypedefs* item = WebKit::core(self);
    WebCore::DOMString[]* convertedValues = WebKit::core(values);
    WebCore::ExceptionCode ec = 0;
    RefPtr<WebCore::DOMString[]> gobjectResult = WTF::getPtr(item->stringArrayFunction2(convertedValues, ec));
    if (ec) {
        WebCore::ExceptionCodeDescription ecdesc(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), ecdesc.code, ecdesc.name);
    }
    return WebKit::kit(gobjectResult.get());
}

void webkit_dom_test_typedefs_method_with_exception(WebKitDOMTestTypedefs* self, GError** error)
{
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self));
    g_return_if_fail(!error || !*error);
    WebCore::TestTypedefs* item = WebKit::core(self);
    WebCore::ExceptionCode ec = 0;
    item->methodWithException(ec);
    if (ec) {
        WebCore::ExceptionCodeDescription ecdesc(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), ecdesc.code, ecdesc.name);
    }
}

guint64 webkit_dom_test_typedefs_get_unsigned_long_long_attr(WebKitDOMTestTypedefs* self)
{
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self), 0);
    WebCore::TestTypedefs* item = WebKit::core(self);
    guint64 result = item->unsignedLongLongAttr();
    return result;
}

void webkit_dom_test_typedefs_set_unsigned_long_long_attr(WebKitDOMTestTypedefs* self, guint64 value)
{
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self));
    WebCore::TestTypedefs* item = WebKit::core(self);
    item->setUnsignedLongLongAttr(value);
}

WebKitDOMSerializedScriptValue* webkit_dom_test_typedefs_get_immutable_serialized_script_value(WebKitDOMTestTypedefs* self)
{
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self), 0);
    WebCore::TestTypedefs* item = WebKit::core(self);
    RefPtr<WebCore::SerializedScriptValue> gobjectResult = WTF::getPtr(item->immutableSerializedScriptValue());
    return WebKit::kit(gobjectResult.get());
}

void webkit_dom_test_typedefs_set_immutable_serialized_script_value(WebKitDOMTestTypedefs* self, WebKitDOMSerializedScriptValue* value)
{
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self));
    g_return_if_fail(WEBKIT_DOM_IS_SERIALIZED_SCRIPT_VALUE(value));
    WebCore::TestTypedefs* item = WebKit::core(self);
    WebCore::SerializedScriptValue* convertedValue = WebKit::core(value);
    item->setImmutableSerializedScriptValue(convertedValue);
}

glong webkit_dom_test_typedefs_get_attr_with_getter_exception(WebKitDOMTestTypedefs* self, GError** error)
{
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    WebCore::TestTypedefs* item = WebKit::core(self);
    WebCore::ExceptionCode ec = 0;
    glong result = item->attrWithGetterException(ec);
    if (ec) {
        WebCore::ExceptionCodeDescription ecdesc(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), ecdesc.code, ecdesc.name);
    }
    return result;
}

void webkit_dom_test_typedefs_set_attr_with_getter_exception(WebKitDOMTestTypedefs* self, glong value)
{
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self));
    WebCore::TestTypedefs* item = WebKit::core(self);
    item->setAttrWithGetterException(value);
}

glong webkit_dom_test_typedefs_get_attr_with_setter_exception(WebKitDOMTestTypedefs* self)
{
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self), 0);
    WebCore::TestTypedefs* item = WebKit::core(self);
    glong result = item->attrWithSetterException();
    return result;
}

void webkit_dom_test_typedefs_set_attr_with_setter_exception(WebKitDOMTestTypedefs* self, glong value, GError** error)
{
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self));
    g_return_if_fail(!error || !*error);
    WebCore::TestTypedefs* item = WebKit::core(self);
    WebCore::ExceptionCode ec = 0;
    item->setAttrWithSetterException(value, ec);
    if (ec) {
        WebCore::ExceptionCodeDescription ecdesc(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), ecdesc.code, ecdesc.name);
    }
}

gchar* webkit_dom_test_typedefs_get_string_attr_with_getter_exception(WebKitDOMTestTypedefs* self, GError** error)
{
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    WebCore::TestTypedefs* item = WebKit::core(self);
    WebCore::ExceptionCode ec = 0;
    gchar* result = convertToUTF8String(item->stringAttrWithGetterException(ec));
    return result;
}

void webkit_dom_test_typedefs_set_string_attr_with_getter_exception(WebKitDOMTestTypedefs* self, const gchar* value)
{
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self));
    g_return_if_fail(value);
    WebCore::TestTypedefs* item = WebKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setStringAttrWithGetterException(convertedValue);
}

gchar* webkit_dom_test_typedefs_get_string_attr_with_setter_exception(WebKitDOMTestTypedefs* self)
{
    WebCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self), 0);
    WebCore::TestTypedefs* item = WebKit::core(self);
    gchar* result = convertToUTF8String(item->stringAttrWithSetterException());
    return result;
}

void webkit_dom_test_typedefs_set_string_attr_with_setter_exception(WebKitDOMTestTypedefs* self, const gchar* value, GError** error)
{
    WebCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_TEST_TYPEDEFS(self));
    g_return_if_fail(value);
    g_return_if_fail(!error || !*error);
    WebCore::TestTypedefs* item = WebKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    WebCore::ExceptionCode ec = 0;
    item->setStringAttrWithSetterException(convertedValue, ec);
    if (ec) {
        WebCore::ExceptionCodeDescription ecdesc(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), ecdesc.code, ecdesc.name);
    }
}

