/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef JSTestSerializedScriptValueInterface_h
#define JSTestSerializedScriptValueInterface_h

#if ENABLE(Condition1) || ENABLE(Condition2)

#include "JSDOMBinding.h"
#include "TestSerializedScriptValueInterface.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSTestSerializedScriptValueInterface : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSTestSerializedScriptValueInterface* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<TestSerializedScriptValueInterface> impl)
    {
        JSTestSerializedScriptValueInterface* ptr = new (NotNull, JSC::allocateCell<JSTestSerializedScriptValueInterface>(globalObject->globalData().heap)) JSTestSerializedScriptValueInterface(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void put(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static void destroy(JSC::JSCell*);
    ~JSTestSerializedScriptValueInterface();
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
    JSC::WriteBarrier<JSC::Unknown> m_cachedValue;
    JSC::WriteBarrier<JSC::Unknown> m_cachedReadonlyValue;
    static void visitChildren(JSCell*, JSC::SlotVisitor&);

    TestSerializedScriptValueInterface* impl() const { return m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull() { if (m_impl) { m_impl->deref(); m_impl = 0; } }

private:
    TestSerializedScriptValueInterface* m_impl;
protected:
    JSTestSerializedScriptValueInterface(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<TestSerializedScriptValueInterface>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::OverridesVisitChildren | Base::StructureFlags;
};

class JSTestSerializedScriptValueInterfaceOwner : public JSC::WeakHandleOwner {
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, TestSerializedScriptValueInterface*)
{
    DEFINE_STATIC_LOCAL(JSTestSerializedScriptValueInterfaceOwner, jsTestSerializedScriptValueInterfaceOwner, ());
    return &jsTestSerializedScriptValueInterfaceOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, TestSerializedScriptValueInterface*)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, TestSerializedScriptValueInterface*);
TestSerializedScriptValueInterface* toTestSerializedScriptValueInterface(JSC::JSValue);

class JSTestSerializedScriptValueInterfacePrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSTestSerializedScriptValueInterfacePrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestSerializedScriptValueInterfacePrototype* ptr = new (NotNull, JSC::allocateCell<JSTestSerializedScriptValueInterfacePrototype>(globalData.heap)) JSTestSerializedScriptValueInterfacePrototype(globalData, globalObject, structure);
        ptr->finishCreation(globalData);
        return ptr;
    }

    static const JSC::ClassInfo s_info;
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

private:
    JSTestSerializedScriptValueInterfacePrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::OverridesVisitChildren | Base::StructureFlags;
};

class JSTestSerializedScriptValueInterfaceConstructor : public DOMConstructorObject {
private:
    JSTestSerializedScriptValueInterfaceConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSTestSerializedScriptValueInterfaceConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSTestSerializedScriptValueInterfaceConstructor* ptr = new (NotNull, JSC::allocateCell<JSTestSerializedScriptValueInterfaceConstructor>(*exec->heap())) JSTestSerializedScriptValueInterfaceConstructor(structure, globalObject);
        ptr->finishCreation(exec, globalObject);
        return ptr;
    }

    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static const JSC::ClassInfo s_info;
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::ImplementsHasInstance | DOMConstructorObject::StructureFlags;
    static JSC::EncodedJSValue JSC_HOST_CALL constructJSTestSerializedScriptValueInterface(JSC::ExecState*);
    static JSC::ConstructType getConstructData(JSC::JSCell*, JSC::ConstructData&);
};

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsTestSerializedScriptValueInterfacePrototypeFunctionAcceptTransferList(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestSerializedScriptValueInterfacePrototypeFunctionMultiTransferList(JSC::ExecState*);
// Attributes

JSC::JSValue jsTestSerializedScriptValueInterfaceValue(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSTestSerializedScriptValueInterfaceValue(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsTestSerializedScriptValueInterfaceReadonlyValue(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTestSerializedScriptValueInterfaceCachedValue(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSTestSerializedScriptValueInterfaceCachedValue(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsTestSerializedScriptValueInterfacePorts(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTestSerializedScriptValueInterfaceCachedReadonlyValue(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTestSerializedScriptValueInterfaceConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif // ENABLE(Condition1) || ENABLE(Condition2)

#endif
