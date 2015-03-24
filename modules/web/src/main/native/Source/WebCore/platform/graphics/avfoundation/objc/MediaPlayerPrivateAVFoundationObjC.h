/*
 * Copyright (C) 2011, 2012, 2013 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef MediaPlayerPrivateAVFoundationObjC_h
#define MediaPlayerPrivateAVFoundationObjC_h

#if ENABLE(VIDEO) && USE(AVFOUNDATION)

#include "MediaPlayerPrivateAVFoundation.h"
#include <wtf/HashMap.h>

OBJC_CLASS AVAssetImageGenerator;
OBJC_CLASS AVAssetResourceLoadingRequest;
OBJC_CLASS AVMediaSelectionGroup;
OBJC_CLASS AVPlayer;
OBJC_CLASS AVPlayerItem;
OBJC_CLASS AVPlayerItemLegibleOutput;
OBJC_CLASS AVPlayerItemTrack;
OBJC_CLASS AVPlayerItemVideoOutput;
OBJC_CLASS AVPlayerLayer;
OBJC_CLASS AVURLAsset;
OBJC_CLASS NSArray;
OBJC_CLASS NSURLAuthenticationChallenge;
OBJC_CLASS WebCoreAVFMovieObserver;
OBJC_CLASS WebCoreAVFPullDelegate;

typedef struct objc_object* id;

#if HAVE(AVFOUNDATION_LOADER_DELEGATE)
OBJC_CLASS WebCoreAVFLoaderDelegate;
OBJC_CLASS AVAssetResourceLoadingRequest;
#endif

typedef struct CGImage *CGImageRef;
typedef struct __CVBuffer *CVPixelBufferRef;
typedef struct OpaqueVTPixelTransferSession* VTPixelTransferSessionRef;

namespace WebCore {

class WebCoreAVFResourceLoader;
class InbandTextTrackPrivateAVFObjC;
class AudioTrackPrivateAVFObjC;
class VideoTrackPrivateAVFObjC;

class MediaPlayerPrivateAVFoundationObjC : public MediaPlayerPrivateAVFoundation {
public:
    virtual ~MediaPlayerPrivateAVFoundationObjC();

    static void registerMediaEngine(MediaEngineRegistrar);

    void setAsset(id);
    virtual void tracksChanged() override;

#if HAVE(AVFOUNDATION_MEDIA_SELECTION_GROUP)
    RetainPtr<AVPlayerItem> playerItem() const { return m_avPlayerItem; }
    void processCue(NSArray *, double);
    void flushCues();
#endif
    
#if HAVE(AVFOUNDATION_LOADER_DELEGATE)
    bool shouldWaitForLoadingOfResource(AVAssetResourceLoadingRequest*);
    bool shouldWaitForResponseToAuthenticationChallenge(NSURLAuthenticationChallenge*);
    void didCancelLoadingRequest(AVAssetResourceLoadingRequest*);
    void didStopLoadingRequest(AVAssetResourceLoadingRequest *);
#endif

#if ENABLE(ENCRYPTED_MEDIA) || ENABLE(ENCRYPTED_MEDIA_V2)
    static bool extractKeyURIKeyIDAndCertificateFromInitData(Uint8Array* initData, String& keyURI, String& keyID, RefPtr<Uint8Array>& certificate);
#endif

#if ENABLE(ENCRYPTED_MEDIA_V2)
    static RetainPtr<AVAssetResourceLoadingRequest> takeRequestForPlayerAndKeyURI(MediaPlayer*, const String&);
#endif

    void playerItemStatusDidChange(int);
    void playbackLikelyToKeepUpWillChange();
    void playbackLikelyToKeepUpDidChange(bool);
    void playbackBufferEmptyWillChange();
    void playbackBufferEmptyDidChange(bool);
    void playbackBufferFullWillChange();
    void playbackBufferFullDidChange(bool);
    void loadedTimeRangesDidChange(RetainPtr<NSArray>);
    void seekableTimeRangesDidChange(RetainPtr<NSArray>);
    void tracksDidChange(RetainPtr<NSArray>);
    void hasEnabledAudioDidChange(bool);
    void presentationSizeDidChange(FloatSize);
    void durationDidChange(double);
    void rateDidChange(double);

#if HAVE(AVFOUNDATION_VIDEO_OUTPUT)
    void outputMediaDataWillChange(AVPlayerItemVideoOutput*);
#endif

private:
    MediaPlayerPrivateAVFoundationObjC(MediaPlayer*);

    WeakPtr<MediaPlayerPrivateAVFoundationObjC> createWeakPtr() { return m_weakPtrFactory.createWeakPtr(); }

    // engine support
    static PassOwnPtr<MediaPlayerPrivateInterface> create(MediaPlayer*);
    static void getSupportedTypes(HashSet<String>& types);
    static MediaPlayer::SupportsType supportsType(const MediaEngineSupportParameters&);

    static bool isAvailable();

    virtual void cancelLoad();

    virtual PlatformMedia platformMedia() const;

    virtual void platformSetVisible(bool);
    virtual void platformPlay();
    virtual void platformPause();
    virtual float currentTime() const;
    virtual void setVolume(float);
    virtual void setClosedCaptionsVisible(bool);
    virtual void paint(GraphicsContext*, const IntRect&);
    virtual void paintCurrentFrameInContext(GraphicsContext*, const IntRect&);
    virtual PlatformLayer* platformLayer() const;
    virtual bool supportsAcceleratedRendering() const { return true; }
    virtual float mediaTimeForTimeValue(float) const;
    virtual double maximumDurationToCacheMediaTime() const { return 5; }

    virtual void createAVPlayer();
    virtual void createAVPlayerItem();
    virtual void createAVAssetForURL(const String& url);
    virtual MediaPlayerPrivateAVFoundation::ItemStatus playerItemStatus() const;
    virtual MediaPlayerPrivateAVFoundation::AssetStatus assetStatus() const;

    virtual void checkPlayability();
    virtual void updateRate();
    virtual float rate() const;
    virtual void seekToTime(double time, double negativeTolerance, double positiveTolerance);
    virtual unsigned long long totalBytes() const;
    virtual PassRefPtr<TimeRanges> platformBufferedTimeRanges() const;
    virtual double platformMinTimeSeekable() const;
    virtual double platformMaxTimeSeekable() const;
    virtual float platformDuration() const;
    virtual float platformMaxTimeLoaded() const;
    virtual void beginLoadingMetadata();
    virtual void sizeChanged();

    virtual bool hasAvailableVideoFrame() const;

    virtual void createContextVideoRenderer();
    virtual void destroyContextVideoRenderer();

    virtual void createVideoLayer();
    virtual void destroyVideoLayer();

    virtual bool hasContextRenderer() const;
    virtual bool hasLayerRenderer() const;

    virtual void updateVideoLayerGravity() override;

    virtual bool hasSingleSecurityOrigin() const;
    
    void createImageGenerator();
    void destroyImageGenerator();
    RetainPtr<CGImageRef> createImageForTimeInRect(float, const IntRect&);
    void paintWithImageGenerator(GraphicsContext*, const IntRect&);

#if HAVE(AVFOUNDATION_VIDEO_OUTPUT)
    void createVideoOutput();
    void destroyVideoOutput();
    RetainPtr<CVPixelBufferRef> createPixelBuffer();
    void updateLastImage();
    bool videoOutputHasAvailableFrame();
    void paintWithVideoOutput(GraphicsContext*, const IntRect&);
    virtual PassNativeImagePtr nativeImageForCurrentTime() override;
    void waitForVideoOutputMediaDataWillChange();
#endif

#if ENABLE(ENCRYPTED_MEDIA)
    virtual MediaPlayer::MediaKeyException addKey(const String&, const unsigned char*, unsigned, const unsigned char*, unsigned, const String&);
    virtual MediaPlayer::MediaKeyException generateKeyRequest(const String&, const unsigned char*, unsigned);
    virtual MediaPlayer::MediaKeyException cancelKeyRequest(const String&, const String&);
#endif

#if ENABLE(ENCRYPTED_MEDIA_V2)
    PassRefPtr<Uint8Array> generateKeyRequest(const String& sessionId, const String& mimeType, Uint8Array* initData, String& destinationURL, MediaPlayer::MediaKeyException& error, unsigned long& systemCode);
    void releaseKeys(const String& sessionId);
    bool update(const String& sessionId, Uint8Array* key, RefPtr<Uint8Array>& nextMessage, MediaPlayer::MediaKeyException& error, unsigned long& systemCode);
#endif

    virtual String languageOfPrimaryAudioTrack() const override;

#if HAVE(AVFOUNDATION_MEDIA_SELECTION_GROUP)
    void processMediaSelectionOptions();
    AVMediaSelectionGroup* safeMediaSelectionGroupForLegibleMedia();
#endif

    virtual void setCurrentTrack(InbandTextTrackPrivateAVF*) override;
    virtual InbandTextTrackPrivateAVF* currentTrack() const override { return m_currentTrack; }

#if !HAVE(AVFOUNDATION_LEGIBLE_OUTPUT_SUPPORT)
    void processLegacyClosedCaptionsTracks();
#endif

#if ENABLE(VIDEO_TRACK)
    void updateAudioTracks();
    void updateVideoTracks();
#endif

    WeakPtrFactory<MediaPlayerPrivateAVFoundationObjC> m_weakPtrFactory;

    RetainPtr<AVURLAsset> m_avAsset;
    RetainPtr<AVPlayer> m_avPlayer;
    RetainPtr<AVPlayerItem> m_avPlayerItem;
    RetainPtr<AVPlayerLayer> m_videoLayer;
    RetainPtr<WebCoreAVFMovieObserver> m_objcObserver;
    RetainPtr<id> m_timeObserver;
    mutable String m_languageOfPrimaryAudioTrack;
    bool m_videoFrameHasDrawn;
    bool m_haveCheckedPlayability;

    RetainPtr<AVAssetImageGenerator> m_imageGenerator;
#if HAVE(AVFOUNDATION_VIDEO_OUTPUT)
    RetainPtr<AVPlayerItemVideoOutput> m_videoOutput;
    RetainPtr<WebCoreAVFPullDelegate> m_videoOutputDelegate;
    RetainPtr<CGImageRef> m_lastImage;
    dispatch_semaphore_t m_videoOutputSemaphore;
#endif

#if USE(VIDEOTOOLBOX)
    RetainPtr<VTPixelTransferSessionRef> m_pixelTransferSession;
#endif

#if HAVE(AVFOUNDATION_LOADER_DELEGATE)
    friend class WebCoreAVFResourceLoader;
    HashMap<RetainPtr<AVAssetResourceLoadingRequest>, RefPtr<WebCoreAVFResourceLoader>> m_resourceLoaderMap;
    RetainPtr<WebCoreAVFLoaderDelegate> m_loaderDelegate;
    HashMap<String, RetainPtr<AVAssetResourceLoadingRequest>> m_keyURIToRequestMap;
    HashMap<String, RetainPtr<AVAssetResourceLoadingRequest>> m_sessionIDToRequestMap;
#endif

#if HAVE(AVFOUNDATION_MEDIA_SELECTION_GROUP) && HAVE(AVFOUNDATION_LEGIBLE_OUTPUT_SUPPORT)
    RetainPtr<AVPlayerItemLegibleOutput> m_legibleOutput;
#endif

#if ENABLE(VIDEO_TRACK)
    Vector<RefPtr<AudioTrackPrivateAVFObjC>> m_audioTracks;
    Vector<RefPtr<VideoTrackPrivateAVFObjC>> m_videoTracks;
#endif

    InbandTextTrackPrivateAVF* m_currentTrack;

    mutable RetainPtr<NSArray> m_cachedSeekableRanges;
    mutable RetainPtr<NSArray> m_cachedLoadedRanges;
    RetainPtr<NSArray> m_cachedTracks;
    FloatSize m_cachedPresentationSize;
    double m_cachedDuration;
    double m_cachedRate;
    unsigned m_pendingStatusChanges;
    int m_cachedItemStatus;
    bool m_cachedLikelyToKeepUp;
    bool m_cachedBufferEmpty;
    bool m_cachedBufferFull;
    bool m_cachedHasEnabledAudio;
};

}

#endif
#endif
