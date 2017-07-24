/*
 * Copyright (c) 2011, 2017, Oracle and/or its affiliates. All rights reserved.
 */

#pragma once

#include "GraphicsContext.h"
#include "GraphicsContextJava.h"
#include "HTMLPlugInElement.h"
#include "IntSize.h"
#include <wtf/java/JavaEnv.h>
#include "ResourceError.h"
#include "ResourceResponse.h"
#include "ScrollView.h"
#include "Widget.h"

#include <wtf/text/WTFString.h>


namespace WebCore {

class PluginWidgetJava final : public Widget {
    RefPtr<HTMLPlugInElement> m_element;
    String m_url;
    String m_mimeType;
    IntSize m_size;
    Vector<String> m_paramNames;
    Vector<String> m_paramValues;

public:
    PluginWidgetJava(
        jobject wfh,
        HTMLPlugInElement* element,
        const IntSize& size,
        const String& url,
        const String& mimeType,
        const Vector<String>& paramNames,
        const Vector<String>& paramValues);
    ~PluginWidgetJava() override;

    void invalidateRect(const IntRect&) override;
    void paint(GraphicsContext&, const IntRect&, SecurityOriginPaintPolicy = SecurityOriginPaintPolicy::AnyOrigin) override;
    void invalidateWindowlessPluginRect(const IntRect& rect);
    void convertToPage(IntRect& rect);
    void focusPluginElement(bool isFocused);
    bool isVisible() {
        return isSelfVisible() && (nullptr == parent() || parent()->isSelfVisible());
    }
    void setFrameRect(const IntRect& rect) override;
    void frameRectsChanged() override;
    void updatePluginWidget();

    //virtual void setFocus();
    //virtual void show();
    //virtual void hide();
    //virtual void paint(GraphicsContext*, const IntRect&);

    // This method is used by plugins on all platforms to obtain a clip rect that includes clips set by WebCore,
    // e.g., in overflow:auto sections.  The clip rects coordinates are in the containing window's coordinate space.
    // This clip includes any clips that the widget itself sets up for its children.
    //IntRect windowClipRect() const;

    void handleEvent(Event*) override;
    //virtual void setParent(ScrollView*);//postponed init have to be implemented (just on non-null parent)
    //virtual void setParentVisible(bool);//pause in rendering

    //virtual bool isPluginView() const { return true; }
};
} // namespace WebCore
