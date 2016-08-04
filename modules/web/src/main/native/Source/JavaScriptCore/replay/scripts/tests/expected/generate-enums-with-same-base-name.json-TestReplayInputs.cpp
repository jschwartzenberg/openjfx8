/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// DO NOT EDIT THIS FILE. It is automatically generated from generate-enums-with-same-base-name.json
// by the script: JavaScriptCore/replay/scripts/CodeGeneratorReplayInputs.py

#include "config.h"
#include "generate-enums-with-same-base-name.json-TestReplayInputs.h"

#if ENABLE(WEB_REPLAY)
#include "InternalNamespaceImplIncludeDummy.h"
#include <platform/ExternalNamespaceImplIncludeDummy.h>

namespace Test {
FormCombo::FormCombo(PlatformEvent1::Type eventType1, PlatformEvent2::Type eventType2, FormData1::Type formType1, FormData2::Type formType2)
    : NondeterministicInput<FormCombo>()
    , m_eventType1(eventType1)
    , m_eventType2(eventType2)
    , m_formType1(formType1)
    , m_formType2(formType2)
{
}

FormCombo::~FormCombo()
{
}
} // namespace Test

namespace JSC {
const String& InputTraits<Test::FormCombo>::type()
{
    static NeverDestroyed<const String> type(ASCIILiteral("FormCombo"));
    return type;
}

void InputTraits<Test::FormCombo>::encode(EncodedValue& encodedValue, const Test::FormCombo& input)
{
    encodedValue.put<PlatformEvent1::Type>(ASCIILiteral("eventType1"), input.eventType1());
    encodedValue.put<PlatformEvent2::Type>(ASCIILiteral("eventType2"), input.eventType2());
    encodedValue.put<Test::FormData1::Type>(ASCIILiteral("formType1"), input.formType1());
    encodedValue.put<Test::FormData2::Type>(ASCIILiteral("formType2"), input.formType2());
}

bool InputTraits<Test::FormCombo>::decode(EncodedValue& encodedValue, std::unique_ptr<Test::FormCombo>& input)
{
    PlatformEvent1::Type eventType1;
    if (!encodedValue.get<PlatformEvent1::Type>(ASCIILiteral("eventType1"), eventType1))
        return false;

    PlatformEvent2::Type eventType2;
    if (!encodedValue.get<PlatformEvent2::Type>(ASCIILiteral("eventType2"), eventType2))
        return false;

    Test::FormData1::Type formType1;
    if (!encodedValue.get<Test::FormData1::Type>(ASCIILiteral("formType1"), formType1))
        return false;

    Test::FormData2::Type formType2;
    if (!encodedValue.get<Test::FormData2::Type>(ASCIILiteral("formType2"), formType2))
        return false;

    input = std::make_unique<Test::FormCombo>(eventType1, eventType2, formType1, formType2);
    return true;
}
EncodedValue EncodingTraits<Test::FormData1::Type>::encodeValue(const Test::FormData1::Type& enumValue)
{
    EncodedValue encodedValue = EncodedValue::createArray();
    if (enumValue & Test::FormData1::Text) {
        encodedValue.append<String>(ASCIILiteral("Text"));
        if (enumValue == Test::FormData1::Text)
            return encodedValue;
    }
    if (enumValue & Test::FormData1::Blob) {
        encodedValue.append<String>(ASCIILiteral("Blob"));
        if (enumValue == Test::FormData1::Blob)
            return encodedValue;
    }
    return encodedValue;
}

bool EncodingTraits<Test::FormData1::Type>::decodeValue(EncodedValue& encodedValue, Test::FormData1::Type& enumValue)
{
    Vector<String> enumStrings;
    if (!EncodingTraits<Vector<String>>::decodeValue(encodedValue, enumStrings))
        return false;

    for (const String& enumString : enumStrings) {
        if (enumString == "Text")
            enumValue = static_cast<Test::FormData1::Type>(enumValue | Test::FormData1::Text);
        else if (enumString == "Blob")
            enumValue = static_cast<Test::FormData1::Type>(enumValue | Test::FormData1::Blob);
    }

    return true;
}

EncodedValue EncodingTraits<Test::FormData2::Type>::encodeValue(const Test::FormData2::Type& enumValue)
{
    switch (enumValue) {
    case Test::FormData2::Type::Text: return EncodedValue::createString("Text");
    case Test::FormData2::Type::Blob: return EncodedValue::createString("Blob");
    default: ASSERT_NOT_REACHED(); return EncodedValue::createString("Error!");
    }
}

bool EncodingTraits<Test::FormData2::Type>::decodeValue(EncodedValue& encodedValue, Test::FormData2::Type& enumValue)
{
    String enumString = encodedValue.convertTo<String>();
    if (enumString == "Text") {
        enumValue = Test::FormData2::Type::Text;
        return true;
    }
    if (enumString == "Blob") {
        enumValue = Test::FormData2::Type::Blob;
        return true;
    }
    return false;
}
} // namespace JSC

#endif // ENABLE(WEB_REPLAY)
