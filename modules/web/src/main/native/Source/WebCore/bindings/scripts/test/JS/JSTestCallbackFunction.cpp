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

#include "config.h"

#if ENABLE(SPEECH_SYNTHESIS)

#include "JSTestCallbackFunction.h"

#include "DOMStringList.h"
#include "JSDOMStringList.h"
#include "JSTestNode.h"
#include "ScriptExecutionContext.h"
#include "SerializedScriptValue.h"
#include "TestNode.h"
#include "URL.h"
#include <runtime/JSLock.h>
#include <runtime/JSString.h>

using namespace JSC;

namespace WebCore {

JSTestCallbackFunction::JSTestCallbackFunction(JSObject* callback, JSDOMGlobalObject* globalObject)
    : TestCallbackFunction()
    , ActiveDOMCallback(globalObject->scriptExecutionContext())
    , m_data(new JSCallbackDataStrong(callback, this))
{
}

JSTestCallbackFunction::~JSTestCallbackFunction()
{
    ScriptExecutionContext* context = scriptExecutionContext();
    // When the context is destroyed, all tasks with a reference to a callback
    // should be deleted. So if the context is 0, we are on the context thread.
    if (!context || context->isContextThread())
        delete m_data;
    else
        context->postTask(DeleteCallbackDataTask(m_data));
#ifndef NDEBUG
    m_data = nullptr;
#endif
}


// Functions

bool JSTestCallbackFunction::callbackWithNoParam()
{
    if (!canInvokeCallback())
        return true;

    Ref<JSTestCallbackFunction> protect(*this);

    JSLockHolder lock(m_data->globalObject()->vm());

    ExecState* state = m_data->globalObject()->globalExec();
    MarkedArgumentBuffer args;

    NakedPtr<Exception> returnedException;
    UNUSED_PARAM(state);
    m_data->invokeCallback(args, JSCallbackData::CallbackType::Function, Identifier(), returnedException);
    if (returnedException)
        reportException(state, returnedException);
    return !returnedException;
}

bool JSTestCallbackFunction::callbackWithArrayParam(RefPtr<Float32Array> arrayParam)
{
    if (!canInvokeCallback())
        return true;

    Ref<JSTestCallbackFunction> protect(*this);

    JSLockHolder lock(m_data->globalObject()->vm());

    ExecState* state = m_data->globalObject()->globalExec();
    MarkedArgumentBuffer args;
    args.append(toJS(state, m_data->globalObject(), WTF::getPtr(arrayParam)));

    NakedPtr<Exception> returnedException;
    UNUSED_PARAM(state);
    m_data->invokeCallback(args, JSCallbackData::CallbackType::Function, Identifier(), returnedException);
    if (returnedException)
        reportException(state, returnedException);
    return !returnedException;
}

bool JSTestCallbackFunction::callbackWithSerializedScriptValueParam(PassRefPtr<SerializedScriptValue> srzParam, const String& strArg)
{
    if (!canInvokeCallback())
        return true;

    Ref<JSTestCallbackFunction> protect(*this);

    JSLockHolder lock(m_data->globalObject()->vm());

    ExecState* state = m_data->globalObject()->globalExec();
    MarkedArgumentBuffer args;
    args.append(srzParam ? srzParam->deserialize(state, castedThis->globalObject(), 0) : jsNull());
    args.append(jsStringWithCache(state, strArg));

    NakedPtr<Exception> returnedException;
    UNUSED_PARAM(state);
    m_data->invokeCallback(args, JSCallbackData::CallbackType::Function, Identifier(), returnedException);
    if (returnedException)
        reportException(state, returnedException);
    return !returnedException;
}

bool JSTestCallbackFunction::callbackWithStringList(PassRefPtr<DOMStringList> listParam)
{
    if (!canInvokeCallback())
        return true;

    Ref<JSTestCallbackFunction> protect(*this);

    JSLockHolder lock(m_data->globalObject()->vm());

    ExecState* state = m_data->globalObject()->globalExec();
    MarkedArgumentBuffer args;
    args.append(toJS(state, m_data->globalObject(), WTF::getPtr(listParam)));

    NakedPtr<Exception> returnedException;
    UNUSED_PARAM(state);
    m_data->invokeCallback(args, JSCallbackData::CallbackType::Function, Identifier(), returnedException);
    if (returnedException)
        reportException(state, returnedException);
    return !returnedException;
}

bool JSTestCallbackFunction::callbackWithBoolean(bool boolParam)
{
    if (!canInvokeCallback())
        return true;

    Ref<JSTestCallbackFunction> protect(*this);

    JSLockHolder lock(m_data->globalObject()->vm());

    ExecState* state = m_data->globalObject()->globalExec();
    MarkedArgumentBuffer args;
    args.append(jsBoolean(boolParam));

    NakedPtr<Exception> returnedException;
    UNUSED_PARAM(state);
    m_data->invokeCallback(args, JSCallbackData::CallbackType::Function, Identifier(), returnedException);
    if (returnedException)
        reportException(state, returnedException);
    return !returnedException;
}

bool JSTestCallbackFunction::callbackRequiresThisToPass(int longParam, TestNode* testNodeParam)
{
    if (!canInvokeCallback())
        return true;

    Ref<JSTestCallbackFunction> protect(*this);

    JSLockHolder lock(m_data->globalObject()->vm());

    ExecState* state = m_data->globalObject()->globalExec();
    MarkedArgumentBuffer args;
    args.append(jsNumber(longParam));
    args.append(toJS(state, m_data->globalObject(), WTF::getPtr(testNodeParam)));

    NakedPtr<Exception> returnedException;
    UNUSED_PARAM(state);
    m_data->invokeCallback(args, JSCallbackData::CallbackType::Function, Identifier(), returnedException);
    if (returnedException)
        reportException(state, returnedException);
    return !returnedException;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, TestCallbackFunction* impl)
{
    if (!impl || !static_cast<JSTestCallbackFunction&>(*impl).callbackData())
        return jsNull();

    return static_cast<JSTestCallbackFunction&>(*impl).callbackData()->callback();

}

}

#endif // ENABLE(SPEECH_SYNTHESIS)
