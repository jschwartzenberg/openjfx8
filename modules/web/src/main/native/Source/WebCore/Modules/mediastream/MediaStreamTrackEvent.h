/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MediaStreamTrackEvent_h
#define MediaStreamTrackEvent_h

#if ENABLE(MEDIA_STREAM)

#include "Event.h"
#include <wtf/text/AtomicString.h>

namespace WebCore {

class MediaStreamTrack;

struct MediaStreamTrackEventInit : public EventInit {
    MediaStreamTrackEventInit();
    
    RefPtr<MediaStreamTrack> track;
};

class MediaStreamTrackEvent : public Event {
public:
    virtual ~MediaStreamTrackEvent();

    static PassRefPtr<MediaStreamTrackEvent> create();
    static PassRefPtr<MediaStreamTrackEvent> create(const AtomicString& type, bool canBubble, bool cancelable, PassRefPtr<MediaStreamTrack>);
    static PassRefPtr<MediaStreamTrackEvent> create(const AtomicString& type, const MediaStreamTrackEventInit& initializer);

    MediaStreamTrack* track() const;

    // Event
    virtual EventInterface eventInterface() const override;

private:
    MediaStreamTrackEvent();
    MediaStreamTrackEvent(const AtomicString& type, bool canBubble, bool cancelable, PassRefPtr<MediaStreamTrack>);
    MediaStreamTrackEvent(const AtomicString& type, const MediaStreamTrackEventInit&);

    RefPtr<MediaStreamTrack> m_track;
};

} // namespace WebCore

#endif // ENABLE(MEDIA_STREAM)

#endif // MediaStreamTrackEvent_h
