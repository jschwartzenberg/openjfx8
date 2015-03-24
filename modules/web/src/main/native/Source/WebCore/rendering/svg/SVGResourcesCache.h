/*
 * Copyright (C) Research In Motion Limited 2010. All rights reserved.
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
 */

#ifndef SVGResourcesCache_h
#define SVGResourcesCache_h

#include "RenderStyleConstants.h"
#include <wtf/HashMap.h>
#include <wtf/OwnPtr.h>

namespace WebCore {

class RenderElement;
class RenderObject;
class RenderStyle;
class RenderSVGResourceContainer;
class SVGResources;

class SVGResourcesCache {
    WTF_MAKE_NONCOPYABLE(SVGResourcesCache); WTF_MAKE_FAST_ALLOCATED;
public:
    SVGResourcesCache();
    ~SVGResourcesCache();

    static SVGResources* cachedResourcesForRenderObject(const RenderObject&);

    // Called from all SVG renderers addChild() methods.
    static void clientWasAddedToTree(RenderObject&);

    // Called from all SVG renderers removeChild() methods.
    static void clientWillBeRemovedFromTree(RenderObject&);

    // Called from all SVG renderers destroy() methods - except for RenderSVGResourceContainer.
    static void clientDestroyed(RenderElement&);

    // Called from all SVG renderers layout() methods.
    static void clientLayoutChanged(RenderElement&);

    // Called from all SVG renderers styleDidChange() methods.
    static void clientStyleChanged(RenderElement&, StyleDifference, const RenderStyle& newStyle);

    // Called from RenderSVGResourceContainer::willBeDestroyed().
    static void resourceDestroyed(RenderSVGResourceContainer&);

private:
    void addResourcesFromRenderer(RenderElement&, const RenderStyle&);
    void removeResourcesFromRenderer(RenderElement&);

    typedef HashMap<const RenderObject*, OwnPtr<SVGResources>> CacheMap;
    CacheMap m_cache;
};

}

#endif
