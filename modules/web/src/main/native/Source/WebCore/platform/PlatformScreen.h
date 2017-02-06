/*
 * Copyright (C) 2006 Apple Inc.  All rights reserved.
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

#ifndef PlatformScreen_h
#define PlatformScreen_h

#include <wtf/Vector.h>

#if PLATFORM(MAC)
OBJC_CLASS NSScreen;
OBJC_CLASS NSWindow;
#ifdef NSGEOMETRY_TYPES_SAME_AS_CGGEOMETRY_TYPES
typedef struct CGRect NSRect;
typedef struct CGPoint NSPoint;
#else
typedef struct _NSRect NSRect;
typedef struct _NSPoint NSPoint;
#endif
#endif

#if PLATFORM(IOS)
OBJC_CLASS UIScreen;
#endif

typedef uint32_t PlatformDisplayID;

typedef WTF::Vector<char> ColorProfile;

namespace WebCore {

    class FloatRect;
    class FloatSize;
    class Widget;

    int screenDepth(Widget*);
    int screenDepthPerComponent(Widget*);
    bool screenIsMonochrome(Widget*);

    bool screenHasInvertedColors();

    FloatRect screenRect(Widget*);
    FloatRect screenAvailableRect(Widget*);

#if PLATFORM(MAC)
    NSScreen *screenForWindow(NSWindow *);

    WEBCORE_EXPORT FloatRect toUserSpace(const NSRect&, NSWindow *destination);
    WEBCORE_EXPORT NSRect toDeviceSpace(const FloatRect&, NSWindow *source);

    NSPoint flipScreenPoint(const NSPoint&, NSScreen *);
#endif

#if PLATFORM(IOS)
    float screenPPIFactor();
    WEBCORE_EXPORT FloatSize screenSize();
    WEBCORE_EXPORT FloatSize availableScreenSize();
    WEBCORE_EXPORT float screenScaleFactor(UIScreen * = nullptr);
#endif

} // namespace WebCore

#endif // PlatformScreen_h
