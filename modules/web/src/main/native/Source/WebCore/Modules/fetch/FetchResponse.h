/*
 * Copyright (C) 2016 Canon Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions
 * are required to be met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Canon Inc. nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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

#ifndef FetchResponse_h
#define FetchResponse_h

#if ENABLE(FETCH_API)

#include "FetchBody.h"
#include "FetchHeaders.h"
#include "ResourceResponse.h"

namespace JSC {
class ArrayBuffer;
};

namespace WebCore {

class Dictionary;

typedef int ExceptionCode;

class FetchResponse : public RefCounted<FetchResponse> {
public:
    static Ref<FetchResponse> create() { return adoptRef(*new FetchResponse(Type::Default, { }, FetchHeaders::create(FetchHeaders::Guard::Response), ResourceResponse())); }
    static Ref<FetchResponse> error();
    static RefPtr<FetchResponse> redirect(ScriptExecutionContext*, const String&, int, ExceptionCode&);
    // FIXME: Binding generator should not require below method to handle optional status parameter.
    static RefPtr<FetchResponse> redirect(ScriptExecutionContext* context, const String& url, ExceptionCode& ec) { return redirect(context, url, 302, ec); }

    void initializeWith(const Dictionary&, ExceptionCode&);

    String type() const;
    const String& url() const { return m_response.url().string(); }
    bool redirected() const { return m_isRedirected; }
    int status() const { return m_response.httpStatusCode(); }
    bool ok() const { return status() >= 200 && status() <= 299; }
    const String& statusText() const { return m_response.httpStatusText(); }

    FetchHeaders& headers() { return m_headers; }
    RefPtr<FetchResponse> clone(ExceptionCode&);

    // Body API
    bool isDisturbed() const { return m_body.isDisturbed(); }
    void arrayBuffer(FetchBody::ArrayBufferPromise&& promise) { m_body.arrayBuffer(WTFMove(promise)); }
    void formData(FetchBody::FormDataPromise&& promise) { m_body.formData(WTFMove(promise)); }
    void blob(FetchBody::BlobPromise&& promise) { m_body.blob(WTFMove(promise)); }
    void json(JSC::ExecState& state, FetchBody::JSONPromise&& promise) { m_body.json(state, WTFMove(promise)); }
    void text(FetchBody::TextPromise&& promise) { m_body.text(WTFMove(promise)); }

private:
    enum class Type { Basic, Cors, Default, Error, Opaque, OpaqueRedirect };

    FetchResponse(Type, FetchBody&&, Ref<FetchHeaders>&&, ResourceResponse&&);

    Type m_type;
    ResourceResponse m_response;
    FetchBody m_body;
    Ref<FetchHeaders> m_headers;
    bool m_isLocked = false;
    bool m_isRedirected = false;
};

} // namespace WebCore

#endif // ENABLE(FETCH_API)

#endif // FetchResponse_h
