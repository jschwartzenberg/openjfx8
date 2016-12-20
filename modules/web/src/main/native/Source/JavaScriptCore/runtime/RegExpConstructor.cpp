/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2007, 2008, 2016 Apple Inc. All Rights Reserved.
 *  Copyright (C) 2009 Torch Mobile, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "config.h"
#include "RegExpConstructor.h"

#include "Error.h"
#include "GetterSetter.h"
#include "JSCInlines.h"
#include "RegExpMatchesArray.h"
#include "RegExpPrototype.h"
#include "StructureInlines.h"

namespace JSC {

static EncodedJSValue regExpConstructorInput(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorMultiline(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorLastMatch(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorLastParen(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorLeftContext(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorRightContext(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorDollar1(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorDollar2(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorDollar3(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorDollar4(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorDollar5(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorDollar6(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorDollar7(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorDollar8(ExecState*, EncodedJSValue, PropertyName);
static EncodedJSValue regExpConstructorDollar9(ExecState*, EncodedJSValue, PropertyName);

static void setRegExpConstructorInput(ExecState*, EncodedJSValue, EncodedJSValue);
static void setRegExpConstructorMultiline(ExecState*, EncodedJSValue, EncodedJSValue);

} // namespace JSC

#include "RegExpConstructor.lut.h"

namespace JSC {

const ClassInfo RegExpConstructor::s_info = { "Function", &InternalFunction::s_info, &regExpConstructorTable, CREATE_METHOD_TABLE(RegExpConstructor) };

/* Source for RegExpConstructor.lut.h
@begin regExpConstructorTable
    input           regExpConstructorInput          None
    $_              regExpConstructorInput          DontEnum
    multiline       regExpConstructorMultiline      None
    $*              regExpConstructorMultiline      DontEnum
    lastMatch       regExpConstructorLastMatch      DontDelete|ReadOnly
    $&              regExpConstructorLastMatch      DontDelete|ReadOnly|DontEnum
    lastParen       regExpConstructorLastParen      DontDelete|ReadOnly
    $+              regExpConstructorLastParen      DontDelete|ReadOnly|DontEnum
    leftContext     regExpConstructorLeftContext    DontDelete|ReadOnly
    $`              regExpConstructorLeftContext    DontDelete|ReadOnly|DontEnum
    rightContext    regExpConstructorRightContext   DontDelete|ReadOnly
    $'              regExpConstructorRightContext   DontDelete|ReadOnly|DontEnum
    $1              regExpConstructorDollar1        DontDelete|ReadOnly
    $2              regExpConstructorDollar2        DontDelete|ReadOnly
    $3              regExpConstructorDollar3        DontDelete|ReadOnly
    $4              regExpConstructorDollar4        DontDelete|ReadOnly
    $5              regExpConstructorDollar5        DontDelete|ReadOnly
    $6              regExpConstructorDollar6        DontDelete|ReadOnly
    $7              regExpConstructorDollar7        DontDelete|ReadOnly
    $8              regExpConstructorDollar8        DontDelete|ReadOnly
    $9              regExpConstructorDollar9        DontDelete|ReadOnly
@end
*/

RegExpConstructor::RegExpConstructor(VM& vm, Structure* structure, RegExpPrototype* regExpPrototype)
    : InternalFunction(vm, structure)
    , m_cachedResult(vm, this, regExpPrototype->regExp())
    , m_multiline(false)
{
}

void RegExpConstructor::finishCreation(VM& vm, RegExpPrototype* regExpPrototype, GetterSetter* speciesSymbol)
{
    Base::finishCreation(vm, regExpPrototype->classInfo()->className);
    ASSERT(inherits(info()));

    // ECMA 15.10.5.1 RegExp.prototype
    putDirectWithoutTransition(vm, vm.propertyNames->prototype, regExpPrototype, DontEnum | DontDelete | ReadOnly);

    // no. of arguments for constructor
    putDirectWithoutTransition(vm, vm.propertyNames->length, jsNumber(2), ReadOnly | DontDelete | DontEnum);

    putDirectNonIndexAccessor(vm, vm.propertyNames->speciesSymbol, speciesSymbol, Accessor | ReadOnly | DontEnum);
}

void RegExpConstructor::destroy(JSCell* cell)
{
    static_cast<RegExpConstructor*>(cell)->RegExpConstructor::~RegExpConstructor();
}

void RegExpConstructor::visitChildren(JSCell* cell, SlotVisitor& visitor)
{
    RegExpConstructor* thisObject = jsCast<RegExpConstructor*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());
    Base::visitChildren(thisObject, visitor);
    thisObject->m_cachedResult.visitChildren(visitor);
}

JSValue RegExpConstructor::getBackref(ExecState* exec, unsigned i)
{
    JSArray* array = m_cachedResult.lastResult(exec, this);

    if (i < array->length()) {
        JSValue result = JSValue(array).get(exec, i);
        ASSERT(result.isString() || result.isUndefined());
        if (!result.isUndefined())
            return result;
    }
    return jsEmptyString(exec);
}

JSValue RegExpConstructor::getLastParen(ExecState* exec)
{
    JSArray* array = m_cachedResult.lastResult(exec, this);
    unsigned length = array->length();
    if (length > 1) {
        JSValue result = JSValue(array).get(exec, length - 1);
        ASSERT(result.isString() || result.isUndefined());
        if (!result.isUndefined())
            return result;
    }
    return jsEmptyString(exec);
}

JSValue RegExpConstructor::getLeftContext(ExecState* exec)
{
    return m_cachedResult.leftContext(exec, this);
}

JSValue RegExpConstructor::getRightContext(ExecState* exec)
{
    return m_cachedResult.rightContext(exec, this);
}

bool RegExpConstructor::getOwnPropertySlot(JSObject* object, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<RegExpConstructor, InternalFunction>(exec, regExpConstructorTable, jsCast<RegExpConstructor*>(object), propertyName, slot);
}

EncodedJSValue regExpConstructorDollar1(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getBackref(exec, 1));
}

EncodedJSValue regExpConstructorDollar2(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getBackref(exec, 2));
}

EncodedJSValue regExpConstructorDollar3(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getBackref(exec, 3));
}

EncodedJSValue regExpConstructorDollar4(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getBackref(exec, 4));
}

EncodedJSValue regExpConstructorDollar5(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getBackref(exec, 5));
}

EncodedJSValue regExpConstructorDollar6(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getBackref(exec, 6));
}

EncodedJSValue regExpConstructorDollar7(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getBackref(exec, 7));
}

EncodedJSValue regExpConstructorDollar8(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getBackref(exec, 8));
}

EncodedJSValue regExpConstructorDollar9(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getBackref(exec, 9));
}

EncodedJSValue regExpConstructorInput(ExecState*, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->input());
}

EncodedJSValue regExpConstructorMultiline(ExecState*, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(jsBoolean(asRegExpConstructor(JSValue::decode(thisValue))->multiline()));
}

EncodedJSValue regExpConstructorLastMatch(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getBackref(exec, 0));
}

EncodedJSValue regExpConstructorLastParen(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getLastParen(exec));
}

EncodedJSValue regExpConstructorLeftContext(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getLeftContext(exec));
}

EncodedJSValue regExpConstructorRightContext(ExecState* exec, EncodedJSValue thisValue, PropertyName)
{
    return JSValue::encode(asRegExpConstructor(JSValue::decode(thisValue))->getRightContext(exec));
}

void setRegExpConstructorInput(ExecState* exec, EncodedJSValue thisValue, EncodedJSValue value)
{
    if (auto constructor = jsDynamicCast<RegExpConstructor*>(JSValue::decode(thisValue)))
        constructor->setInput(exec, JSValue::decode(value).toString(exec));
}

void setRegExpConstructorMultiline(ExecState* exec, EncodedJSValue thisValue, EncodedJSValue value)
{
    if (auto constructor = jsDynamicCast<RegExpConstructor*>(JSValue::decode(thisValue)))
        constructor->setMultiline(JSValue::decode(value).toBoolean(exec));
}

inline Structure* getRegExpStructure(ExecState* exec, JSGlobalObject* globalObject, JSValue newTarget)
{
    Structure* structure = globalObject->regExpStructure();
    if (newTarget != jsUndefined())
        structure = InternalFunction::createSubclassStructure(exec, newTarget, structure);
    return structure;
}

// ECMA 15.10.4
JSObject* constructRegExp(ExecState* exec, JSGlobalObject* globalObject, const ArgList& args, JSValue newTarget)
{
    JSValue arg0 = args.at(0);
    JSValue arg1 = args.at(1);

    if (arg0.inherits(RegExpObject::info())) {
        if (!arg1.isUndefined())
            return exec->vm().throwException(exec, createTypeError(exec, ASCIILiteral("Cannot supply flags when constructing one RegExp from another.")));
        // If called as a function, this just returns the first argument (see 15.10.3.1).
        if (newTarget != jsUndefined()) {
            RegExp* regExp = static_cast<RegExpObject*>(asObject(arg0))->regExp();
            Structure* structure = getRegExpStructure(exec, globalObject, newTarget);
            if (exec->hadException())
                return nullptr;

            return RegExpObject::create(exec->vm(), structure, regExp);
        }
        return asObject(arg0);
    }

    String pattern = arg0.isUndefined() ? emptyString() : arg0.toString(exec)->value(exec);
    if (exec->hadException())
        return 0;

    RegExpFlags flags = NoFlags;
    if (!arg1.isUndefined()) {
        flags = regExpFlags(arg1.toString(exec)->value(exec));
        if (exec->hadException())
            return 0;
        if (flags == InvalidFlags)
            return exec->vm().throwException(exec, createSyntaxError(exec, ASCIILiteral("Invalid flags supplied to RegExp constructor.")));
    }

    VM& vm = exec->vm();
    RegExp* regExp = RegExp::create(vm, pattern, flags);
    if (!regExp->isValid())
        return vm.throwException(exec, createSyntaxError(exec, regExp->errorMessage()));

    Structure* structure = getRegExpStructure(exec, globalObject, newTarget);
    if (exec->hadException())
        return nullptr;
    return RegExpObject::create(vm, structure, regExp);
}

static EncodedJSValue JSC_HOST_CALL constructWithRegExpConstructor(ExecState* exec)
{
    ArgList args(exec);
    return JSValue::encode(constructRegExp(exec, asInternalFunction(exec->callee())->globalObject(), args, exec->newTarget()));
}

ConstructType RegExpConstructor::getConstructData(JSCell*, ConstructData& constructData)
{
    constructData.native.function = constructWithRegExpConstructor;
    return ConstructTypeHost;
}

// ECMA 15.10.3
static EncodedJSValue JSC_HOST_CALL callRegExpConstructor(ExecState* exec)
{
    ArgList args(exec);
    return JSValue::encode(constructRegExp(exec, asInternalFunction(exec->callee())->globalObject(), args));
}

CallType RegExpConstructor::getCallData(JSCell*, CallData& callData)
{
    callData.native.function = callRegExpConstructor;
    return CallTypeHost;
}

} // namespace JSC
