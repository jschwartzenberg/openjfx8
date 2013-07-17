/*
 * Copyright (C) 2013 Apple Inc. All Rights Reserved.
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

#include "config.h"
#include "SourceProvider.h"
#include <wtf/StdLibExtras.h>
#include <wtf/TCSpinLock.h>

namespace JSC {

SourceProvider::SourceProvider(const String& url, const TextPosition& startPosition)
    : m_url(url)
    , m_startPosition(startPosition)
    , m_validated(false)
    , m_id(0)
{
}

SourceProvider::~SourceProvider()
{
}

Vector<size_t>& SourceProvider::lineStarts()
{
    if (!m_lineStarts) {
        m_lineStarts = adoptPtr(new Vector<size_t>());
        String source = this->source();
        size_t index = 0;
        do {
            m_lineStarts->append(index);
            index = source.findNextLineStart(index);
        } while (index != notFound);
        m_lineStarts->shrinkToFit();
    }
    return *m_lineStarts;
}


static inline size_t charPositionExtractor(const size_t* value)
{
    return *value;
}

size_t SourceProvider::charPositionToColumnNumber(size_t charPosition)
{
    Vector<size_t>& lineStarts = this->lineStarts();
    size_t* data = lineStarts.data();
    size_t dataSize = lineStarts.size();

    // Get the nearest line start entry (which could be to the left or to the
    // right of the requested charPosition.
    const size_t* line = approximateBinarySearch<size_t, size_t>(data, dataSize, charPosition, charPositionExtractor);
    size_t lineStartPosition = *line;

    if (lineStartPosition > charPosition) {
        if (data < line) {
            line--;
            lineStartPosition = *line;
        }
    }

    ASSERT(data <= line);
    ASSERT(lineStartPosition <= charPosition);
    return charPosition - lineStartPosition;
}

static TCMalloc_SpinLock providerIdLock = SPINLOCK_INITIALIZER;

void SourceProvider::getID()
{
    SpinLockHolder lock(&providerIdLock);
    if (!m_id) {
        static intptr_t nextProviderID = 0;
        m_id = ++nextProviderID;
    }
}

} // namespace JSC

