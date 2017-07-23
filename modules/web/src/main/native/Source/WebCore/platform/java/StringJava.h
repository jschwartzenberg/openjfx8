/*
 * Copyright (c) 2011, 2013, Oracle and/or its affiliates. All rights reserved.
 */

#pragma once

#include <wtf/text/WTFString.h>
#include "Font.h"

namespace WebCore {
using WTF::String;
jobjectArray strVect2JArray(
    JNIEnv* env, const Vector<String>& strVect);

} // namespace WebCore
