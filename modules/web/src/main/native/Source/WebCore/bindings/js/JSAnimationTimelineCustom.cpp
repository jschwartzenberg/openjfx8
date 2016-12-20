/*
 * Copyright (C) Canon Inc. 2016
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions
 * are required to be met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Canon Inc. nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY CANON INC. AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CANON INC. AND ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(WEB_ANIMATIONS)
#include "JSAnimationTimeline.h"

#include "DocumentTimeline.h"
#include "JSDOMBinding.h"
#include "JSDocumentTimeline.h"

using namespace JSC;

namespace WebCore {

JSValue toJS(ExecState*, JSDOMGlobalObject* globalObject, AnimationTimeline* value)
{
    if (!value)
        return jsNull();

    JSObject* wrapper = getCachedWrapper(globalObject->world(), value);

    if (wrapper)
        return wrapper;

    if (value->isDocumentTimeline())
        wrapper = CREATE_DOM_WRAPPER(globalObject, DocumentTimeline, value);
    else
        wrapper = CREATE_DOM_WRAPPER(globalObject, AnimationTimeline, value);

    return wrapper;
}

} // namespace WebCore

#endif // ENABLE(WEB_ANIMATIONS)
