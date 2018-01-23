/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <wtf/Deque.h>
#include <wtf/Forward.h>
#include <wtf/HashMap.h>
#include <wtf/Lock.h>
#include <wtf/MainThread.h>

typedef struct _NPP NPP_t;
typedef NPP_t* NPP;

namespace WebCore {

class PluginMainThreadScheduler {
    WTF_MAKE_NONCOPYABLE(PluginMainThreadScheduler); WTF_MAKE_FAST_ALLOCATED;
public:
    typedef void MainThreadFunction(void*);

    WEBCORE_EXPORT static PluginMainThreadScheduler& scheduler();

    WEBCORE_EXPORT void scheduleCall(NPP, MainThreadFunction*, void* userData);

    WEBCORE_EXPORT void registerPlugin(NPP);
    WEBCORE_EXPORT void unregisterPlugin(NPP);

private:
    friend NeverDestroyed<PluginMainThreadScheduler>;
    PluginMainThreadScheduler();
    void dispatchCalls();

    class Call;

    void dispatchCallsForPlugin(NPP, const Deque<Call>& calls);
    typedef HashMap<NPP, Deque<Call>> CallQueueMap;

    class Call {
    public:
        Call(MainThreadFunction* function, void* userData)
            : m_function(function)
            , m_userData(userData)
        {
        }

        void performCall() const
        {
            m_function(m_userData);
        }

    private:
        MainThreadFunction* m_function;
        void* m_userData;
    };

    bool m_callPending;
    CallQueueMap m_callQueueMap;
    Lock m_queueMutex;
};

} // namespace WebCore
