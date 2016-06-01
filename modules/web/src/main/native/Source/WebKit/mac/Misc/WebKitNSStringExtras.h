/*
 * Copyright (C) 2005 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <Foundation/Foundation.h>

#if !TARGET_OS_IPHONE
#import <AppKit/AppKit.h>
#endif

extern NSString *WebKitLocalCacheDefaultsKey;

@interface NSString (WebKitExtras)

#if !TARGET_OS_IPHONE
- (void)_web_drawAtPoint:(NSPoint)point font:(NSFont *)font textColor:(NSColor *)textColor allowingFontSmoothing:(BOOL)fontSmoothingIsAllowed;
- (void)_web_drawAtPoint:(NSPoint)point font:(NSFont *)font textColor:(NSColor *)textColor;
- (void)_web_drawDoubledAtPoint:(NSPoint)textPoint withTopColor:(NSColor *)topColor bottomColor:(NSColor *)bottomColor font:(NSFont *)font;

- (float)_web_widthWithFont:(NSFont *)font;
#endif

// Handles home directories that have symlinks in their paths.
// This works around 2774250.
- (NSString *)_web_stringByAbbreviatingWithTildeInPath;

- (NSString *)_web_stringByStrippingReturnCharacters;

- (BOOL)_webkit_isCaseInsensitiveEqualToString:(NSString *)string;
- (BOOL)_webkit_hasCaseInsensitivePrefix:(NSString *)suffix;
- (BOOL)_webkit_hasCaseInsensitiveSuffix:(NSString *)suffix;
- (BOOL)_webkit_hasCaseInsensitiveSubstring:(NSString *)substring;
- (NSString *)_webkit_filenameByFixingIllegalCharacters;

- (NSString *)_webkit_stringByTrimmingWhitespace;
- (NSString *)_webkit_stringByCollapsingNonPrintingCharacters;
- (NSString *)_webkit_stringByCollapsingWhitespaceCharacters;

#if TARGET_OS_IPHONE
+ (NSString *)_web_stringWithData:(NSData *)data textEncodingName:(NSString *)textEncodingName;
#endif

+ (NSString *)_webkit_localCacheDirectoryWithBundleIdentifier:(NSString*)bundleIdentifier;

@end
