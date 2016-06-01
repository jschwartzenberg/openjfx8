/*
 * Copyright (C) 2014 Apple Inc. All Rights Reserved.
 * Copyright (C) 2011 Anthony Johnson. All Rights Reserved.
 * Copyright (C) 2011 Brent Fulgham. All Rights Reserved.
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

#include "stdafx.h"
#include "DOMDefaultImpl.h"

// IUnknown -------------------------------------------------------------------
HRESULT WebScriptObject::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IUnknown))
        *ppvObject = static_cast<IWebScriptObject*>(this);
    else if (IsEqualGUID(riid, IID_IWebScriptObject))
        *ppvObject = static_cast<IWebScriptObject*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG WebScriptObject::AddRef(void)
{
    return ++m_refCount;
}

ULONG WebScriptObject::Release(void)
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete(this);

    return newRef;
}

// DOMObject -------------------------------------------------------------------
HRESULT DOMObject::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IDOMObject))
        *ppvObject = static_cast<IDOMObject*>(this);
    else
        return WebScriptObject::QueryInterface(riid, ppvObject);

    WebScriptObject::AddRef();
    return S_OK;
}


// DOMEventListener -------------------------------------------------------------------
HRESULT DOMEventListener::QueryInterface(const IID &riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IDOMEventListener))
        *ppvObject = static_cast<IDOMEventListener*>(this);
    else
        return DOMObject::QueryInterface(riid, ppvObject);

    AddRef();
    return S_OK;
}

ULONG DOMEventListener::AddRef(void)
{
    return WebScriptObject::AddRef();
}

ULONG DOMEventListener::Release(void)
{
    return WebScriptObject::Release();
}
