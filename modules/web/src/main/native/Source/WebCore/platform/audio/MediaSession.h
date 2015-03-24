/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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

#ifndef MediaSession_h
#define MediaSession_h

#include <wtf/Noncopyable.h>

namespace WebCore {

class MediaSessionClient;

class MediaSession {
public:
    static std::unique_ptr<MediaSession> create(MediaSessionClient&);

    MediaSession(MediaSessionClient&);
    virtual ~MediaSession();

    enum MediaType {
        None = 0,
        Video,
        Audio,
        WebAudio,
    };
    MediaType mediaType() const;

    enum State {
        Idle,
        Playing,
        Paused,
        Interrupted,
    };
    State state() const { return m_state; }
    void setState(State);

    enum EndInterruptionFlags {
        NoFlags = 0,
        MayResumePlaying = 1 << 0,
    };
    void beginInterruption();
    void endInterruption(EndInterruptionFlags);

    void applicationWillEnterForeground() const;
    void applicationWillEnterBackground() const;

    bool clientWillBeginPlayback();
    bool clientWillPausePlayback();

    void pauseSession();

protected:
    MediaSessionClient& client() const { return m_client; }

private:
    MediaSessionClient& m_client;
    State m_state;
    State m_stateToRestore;
    bool m_notifyingClient;
};

class MediaSessionClient {
    WTF_MAKE_NONCOPYABLE(MediaSessionClient);
public:
    MediaSessionClient() { }
    
    virtual MediaSession::MediaType mediaType() const = 0;
    virtual void resumePlayback() = 0;
    virtual void pausePlayback() = 0;

protected:
    virtual ~MediaSessionClient() { }
};

}

#endif // MediaSession_h
