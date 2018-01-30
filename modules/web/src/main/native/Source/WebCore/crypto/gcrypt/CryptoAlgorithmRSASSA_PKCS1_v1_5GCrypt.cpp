/*
 * Copyright (C) 2014 Igalia S.L. All rights reserved.
 * Copyright (C) 2016 Yusuke Suzuki <utatane.tea@gmail.com>.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "CryptoAlgorithmRSASSA_PKCS1_v1_5.h"

#if ENABLE(SUBTLE_CRYPTO)

#include "CryptoAlgorithmRsaSsaParamsDeprecated.h"
#include "CryptoKeyRSA.h"
#include "GCryptUtilities.h"
#include "NotImplemented.h"
#include "ScriptExecutionContext.h"

namespace WebCore {

static std::optional<Vector<uint8_t>> gcryptSign(gcry_sexp_t keySexp, const Vector<uint8_t>& data, CryptoAlgorithmIdentifier hashAlgorithmIdentifier)
{
    // Perform digest operation with the specified algorithm on the given data.
    Vector<uint8_t> dataHash;
    {
        auto digestAlgorithm = hashCryptoDigestAlgorithm(hashAlgorithmIdentifier);
        if (!digestAlgorithm)
            return std::nullopt;

        auto digest = PAL::CryptoDigest::create(*digestAlgorithm);
        if (!digest)
            return std::nullopt;

        digest->addBytes(data.data(), data.size());
        dataHash = digest->computeHash();
    }

    // Construct the data s-expression that contains PKCS#1-padded hashed data.
    PAL::GCrypt::Handle<gcry_sexp_t> dataSexp;
    {
        auto shaAlgorithm = hashAlgorithmName(hashAlgorithmIdentifier);
        if (!shaAlgorithm)
            return std::nullopt;

        gcry_error_t error = gcry_sexp_build(&dataSexp, nullptr, "(data(flags pkcs1)(hash %s %b))",
            *shaAlgorithm, dataHash.size(), dataHash.data());
        if (error != GPG_ERR_NO_ERROR) {
            PAL::GCrypt::logError(error);
            return std::nullopt;
        }
    }

    // Perform the PK signing, retrieving a sig-val s-expression of the following form:
    // (sig-val
    //   (rsa
    //     (s s-mpi)))
    PAL::GCrypt::Handle<gcry_sexp_t> signatureSexp;
    gcry_error_t error = gcry_pk_sign(&signatureSexp, dataSexp, keySexp);
    if (error != GPG_ERR_NO_ERROR) {
        PAL::GCrypt::logError(error);
        return std::nullopt;
    }

    // Return MPI data of the embedded s integer.
    PAL::GCrypt::Handle<gcry_sexp_t> sSexp(gcry_sexp_find_token(signatureSexp, "s", 0));
    if (!sSexp)
        return std::nullopt;

    return mpiData(sSexp);
}

static std::optional<bool> gcryptVerify(gcry_sexp_t keySexp, const Vector<uint8_t>& signature, const Vector<uint8_t>& data, CryptoAlgorithmIdentifier hashAlgorithmIdentifier)
{
    // Perform digest operation with the specified algorithm on the given data.
    Vector<uint8_t> dataHash;
    {
        auto digestAlgorithm = hashCryptoDigestAlgorithm(hashAlgorithmIdentifier);
        if (!digestAlgorithm)
            return std::nullopt;

        auto digest = PAL::CryptoDigest::create(*digestAlgorithm);
        if (!digest)
            return std::nullopt;

        digest->addBytes(data.data(), data.size());
        dataHash = digest->computeHash();
    }

    // Construct the sig-val s-expression that contains the signature data.
    PAL::GCrypt::Handle<gcry_sexp_t> signatureSexp;
    gcry_error_t error = gcry_sexp_build(&signatureSexp, nullptr, "(sig-val(rsa(s %b)))",
        signature.size(), signature.data());
    if (error != GPG_ERR_NO_ERROR) {
        PAL::GCrypt::logError(error);
        return std::nullopt;
    }

    // Construct the data s-expression that contains PKCS#1-padded hashed data.
    PAL::GCrypt::Handle<gcry_sexp_t> dataSexp;
    {
        auto shaAlgorithm = hashAlgorithmName(hashAlgorithmIdentifier);
        if (!shaAlgorithm)
            return std::nullopt;

        error = gcry_sexp_build(&dataSexp, nullptr, "(data(flags pkcs1)(hash %s %b))",
            *shaAlgorithm, dataHash.size(), dataHash.data());
        if (error != GPG_ERR_NO_ERROR) {
            PAL::GCrypt::logError(error);
            return std::nullopt;
        }
    }

    // Perform the PK verification. We report success if there's no error returned, or
    // a failure in any other case. OperationError should not be returned at this point,
    // avoiding spilling information about the exact cause of verification failure.
    error = gcry_pk_verify(signatureSexp, dataSexp, keySexp);
    return { error == GPG_ERR_NO_ERROR };
}

void CryptoAlgorithmRSASSA_PKCS1_v1_5::platformSign(Ref<CryptoKey>&& key, Vector<uint8_t>&& data, VectorCallback&& callback, ExceptionCallback&& exceptionCallback, ScriptExecutionContext& context, WorkQueue& workQueue)
{
    context.ref();
    workQueue.dispatch(
        [key = WTFMove(key), data = WTFMove(data), callback = WTFMove(callback), exceptionCallback = WTFMove(exceptionCallback), &context]() mutable {
            auto& rsaKey = downcast<CryptoKeyRSA>(key.get());

            auto output = gcryptSign(rsaKey.platformKey(), data, rsaKey.hashAlgorithmIdentifier());
            if (!output) {
                // We should only dereference callbacks after being back to the Document/Worker threads.
                context.postTask(
                    [callback = WTFMove(callback), exceptionCallback = WTFMove(exceptionCallback)](ScriptExecutionContext& context) {
                        exceptionCallback(OperationError);
                        context.deref();
                    });
                return;
            }

            // We should only dereference callbacks after being back to the Document/Worker threads.
            context.postTask(
                [output = WTFMove(*output), callback = WTFMove(callback), exceptionCallback = WTFMove(exceptionCallback)](ScriptExecutionContext& context) {
                    callback(output);
                    context.deref();
                });
        });
}

void CryptoAlgorithmRSASSA_PKCS1_v1_5::platformVerify(Ref<CryptoKey>&& key, Vector<uint8_t>&& signature, Vector<uint8_t>&& data, BoolCallback&& callback, ExceptionCallback&& exceptionCallback, ScriptExecutionContext& context, WorkQueue& workQueue)
{
    context.ref();
    workQueue.dispatch(
        [key = WTFMove(key), signature = WTFMove(signature), data = WTFMove(data), callback = WTFMove(callback), exceptionCallback = WTFMove(exceptionCallback), &context]() mutable {
            auto& rsaKey = downcast<CryptoKeyRSA>(key.get());

            auto output = gcryptVerify(rsaKey.platformKey(), signature, data, rsaKey.hashAlgorithmIdentifier());
            if (!output) {
                // We should only dereference callbacks after being back to the Document/Worker threads.
                context.postTask(
                    [callback = WTFMove(callback), exceptionCallback = WTFMove(exceptionCallback)](ScriptExecutionContext& context) {
                        exceptionCallback(OperationError);
                        context.deref();
                    });
                return;
            }

            // We should only dereference callbacks after being back to the Document/Worker threads.
            context.postTask(
                [output = WTFMove(*output), callback = WTFMove(callback), exceptionCallback = WTFMove(exceptionCallback)](ScriptExecutionContext& context) {
                    callback(output);
                    context.deref();
                });
        });
}

ExceptionOr<void> CryptoAlgorithmRSASSA_PKCS1_v1_5::platformSign(const CryptoAlgorithmRsaSsaParamsDeprecated&, const CryptoKeyRSA&, const CryptoOperationData&, VectorCallback&&, VoidCallback&&)
{
    notImplemented();
    return Exception { NotSupportedError };
}

ExceptionOr<void> CryptoAlgorithmRSASSA_PKCS1_v1_5::platformVerify(const CryptoAlgorithmRsaSsaParamsDeprecated&, const CryptoKeyRSA&, const CryptoOperationData&, const CryptoOperationData&, BoolCallback&&, VoidCallback&&)
{
    notImplemented();
    return Exception { NotSupportedError };
}

} // namespace WebCore

#endif // ENABLE(SUBTLE_CRYPTO)
