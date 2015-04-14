/*
 * Copyright (C) 2000 Lars Knoll (knoll@kde.org)
 *           (C) 2000 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef StyleGeneratedImage_h
#define StyleGeneratedImage_h

#include "StyleImage.h"

namespace WebCore {

class CSSValue;
class CSSImageGeneratorValue;

class StyleGeneratedImage final : public StyleImage {
public:
    static PassRefPtr<StyleGeneratedImage> create(PassRef<CSSImageGeneratorValue> value)
    {
        return adoptRef(new StyleGeneratedImage(std::move(value)));
    }

    CSSImageGeneratorValue& imageValue() { return m_imageGeneratorValue.get(); }

private:
    virtual WrappedImagePtr data() const override { return &m_imageGeneratorValue.get(); }

    virtual PassRefPtr<CSSValue> cssValue() const override;

    virtual LayoutSize imageSize(const RenderElement*, float multiplier) const override;
    virtual bool imageHasRelativeWidth() const override { return !m_fixedSize; }
    virtual bool imageHasRelativeHeight() const override { return !m_fixedSize; }
    virtual void computeIntrinsicDimensions(const RenderElement*, Length& intrinsicWidth, Length& intrinsicHeight, FloatSize& intrinsicRatio) override;
    virtual bool usesImageContainerSize() const override { return !m_fixedSize; }
    virtual void setContainerSizeForRenderer(const RenderElement*, const IntSize& containerSize, float) override { m_containerSize = containerSize; }
    virtual void addClient(RenderElement*) override;
    virtual void removeClient(RenderElement*) override;
    virtual PassRefPtr<Image> image(RenderElement*, const IntSize&) const override;
    virtual bool knownToBeOpaque(const RenderElement*) const override;

    StyleGeneratedImage(PassRef<CSSImageGeneratorValue>);
    
    Ref<CSSImageGeneratorValue> m_imageGeneratorValue;
    IntSize m_containerSize;
    bool m_fixedSize;
};

STYLE_IMAGE_TYPE_CASTS(StyleGeneratedImage, StyleImage, isGeneratedImage)

}
#endif
