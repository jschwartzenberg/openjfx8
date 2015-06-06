/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Peter Kelly (pmk@post.com)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2007 David Smith (catfish.man@gmail.com)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012, 2013 Apple Inc. All rights reserved.
 *           (C) 2007 Eric Seidel (eric@webkit.org)
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

#include "config.h"
#include "StyleResolveTree.h"

#include "AXObjectCache.h"
#include "AnimationController.h"
#include "CSSFontSelector.h"
#include "Element.h"
#include "ElementIterator.h"
#include "ElementRareData.h"
#include "FlowThreadController.h"
#include "InsertionPoint.h"
#include "NodeRenderStyle.h"
#include "NodeRenderingTraversal.h"
#include "NodeTraversal.h"
#include "RenderElement.h"
#include "RenderFullScreen.h"
#include "RenderNamedFlowThread.h"
#include "RenderText.h"
#include "RenderView.h"
#include "RenderWidget.h"
#include "Settings.h"
#include "ShadowRoot.h"
#include "StyleResolveForDocument.h"
#include "StyleResolver.h"
#include "Text.h"

#if PLATFORM(IOS)
#include "CSSFontSelector.h"
#include "WKContentObservation.h"
#endif

namespace WebCore {

namespace Style {

enum DetachType { NormalDetach, ReattachDetach };

static void attachRenderTree(Element&, PassRefPtr<RenderStyle>);
static void attachTextRenderer(Text&);
static void detachRenderTree(Element&, DetachType);
static void resolveTree(Element&, Change);

Change determineChange(const RenderStyle* s1, const RenderStyle* s2)
{
    if (!s1 || !s2)
        return Detach;
    if (s1->display() != s2->display())
        return Detach;
    if (s1->hasPseudoStyle(FIRST_LETTER) != s2->hasPseudoStyle(FIRST_LETTER))
        return Detach;
    // We just detach if a renderer acquires or loses a column-span, since spanning elements
    // typically won't contain much content.
    if (s1->columnSpan() != s2->columnSpan())
        return Detach;
    if (!s1->contentDataEquivalent(s2))
        return Detach;
    // When text-combine property has been changed, we need to prepare a separate renderer object.
    // When text-combine is on, we use RenderCombineText, otherwise RenderText.
    // https://bugs.webkit.org/show_bug.cgi?id=55069
    if (s1->hasTextCombine() != s2->hasTextCombine())
        return Detach;
    // We need to reattach the node, so that it is moved to the correct RenderFlowThread.
    if (s1->flowThread() != s2->flowThread())
        return Detach;
    // When the region thread has changed, we need to prepare a separate render region object.
    if (s1->regionThread() != s2->regionThread())
        return Detach;

    if (*s1 != *s2) {
        if (s1->inheritedNotEqual(s2))
            return Inherit;
        if (s1->hasExplicitlyInheritedProperties() || s2->hasExplicitlyInheritedProperties())
            return Inherit;

        return NoInherit;
    }
    // If the pseudoStyles have changed, we want any StyleChange that is not NoChange
    // because setStyle will do the right thing with anything else.
    if (s1->hasAnyPublicPseudoStyles()) {
        for (PseudoId pseudoId = FIRST_PUBLIC_PSEUDOID; pseudoId < FIRST_INTERNAL_PSEUDOID; pseudoId = static_cast<PseudoId>(pseudoId + 1)) {
            if (s1->hasPseudoStyle(pseudoId)) {
                RenderStyle* ps2 = s2->getCachedPseudoStyle(pseudoId);
                if (!ps2)
                    return NoInherit;
                RenderStyle* ps1 = s1->getCachedPseudoStyle(pseudoId);
                if (!ps1 || *ps1 != *ps2)
                    return NoInherit;
            }
        }
    }

    return NoChange;
}

static bool isRendererReparented(const RenderObject* renderer)
{
    if (!renderer->node()->isElementNode())
        return false;
    if (!renderer->style().flowThread().isEmpty())
        return true;
    return false;
}

static RenderObject* nextSiblingRenderer(const Element& element, const ContainerNode* renderingParentNode)
{
    // Avoid an O(N^2) problem with this function by not checking for
    // nextRenderer() when the parent element hasn't attached yet.
    // FIXME: Why would we get here anyway if parent is not attached?
    if (renderingParentNode && !renderingParentNode->renderer())
        return nullptr;
    for (Node* sibling = NodeRenderingTraversal::nextSibling(&element); sibling; sibling = NodeRenderingTraversal::nextSibling(sibling)) {
        RenderObject* renderer = sibling->renderer();
        if (renderer && !isRendererReparented(renderer))
            return renderer;
    }
    return nullptr;
}

static bool shouldCreateRenderer(const Element& element, const ContainerNode* renderingParent)
{
    if (!element.document().shouldCreateRenderers())
        return false;
    if (!renderingParent)
        return false;
    RenderObject* parentRenderer = renderingParent->renderer();
    if (!parentRenderer)
        return false;
    if (!parentRenderer->canHaveChildren() && !(element.isPseudoElement() && parentRenderer->canHaveGeneratedChildren()))
        return false;
    if (!renderingParent->childShouldCreateRenderer(element))
        return false;
    return true;
}

// Check the specific case of elements that are children of regions but are flowed into a flow thread themselves.
static bool elementInsideRegionNeedsRenderer(Element& element, const ContainerNode* renderingParentNode, RefPtr<RenderStyle>& style)
{
#if ENABLE(CSS_REGIONS)
    // The parent of a region should always be an element.
    const RenderElement* parentRenderer = renderingParentNode ? renderingParentNode->renderer() : 0;

    bool parentIsRegion = parentRenderer && !parentRenderer->canHaveChildren() && parentRenderer->isRenderNamedFlowFragmentContainer();
    bool parentIsNonRenderedInsideRegion = !parentRenderer && element.parentElement() && element.parentElement()->isInsideRegion();
    if (!parentIsRegion && !parentIsNonRenderedInsideRegion)
        return false;

    if (!style)
        style = element.styleForRenderer();

    // Children of this element will only be allowed to be flowed into other flow-threads if display is NOT none.
    if (element.rendererIsNeeded(*style))
        element.setIsInsideRegion(true);

    if (element.shouldMoveToFlowThread(*style))
        return true;
#else
    UNUSED_PARAM(element);
    UNUSED_PARAM(renderingParentNode);
    UNUSED_PARAM(style);
#endif
    return false;
}

#if ENABLE(CSS_REGIONS)
static RenderNamedFlowThread* moveToFlowThreadIfNeeded(Element& element, const RenderStyle& style)
{
    if (!element.shouldMoveToFlowThread(style))
        return 0;
    FlowThreadController& flowThreadController = element.document().renderView()->flowThreadController();
    RenderNamedFlowThread& parentFlowRenderer = flowThreadController.ensureRenderFlowThreadWithName(style.flowThread());
    flowThreadController.registerNamedFlowContentElement(element, parentFlowRenderer);
    return &parentFlowRenderer;
}
#endif

static void createRendererIfNeeded(Element& element, PassRefPtr<RenderStyle> resolvedStyle)
{
    ASSERT(!element.renderer());

    ContainerNode* renderingParentNode = NodeRenderingTraversal::parent(&element);

    RefPtr<RenderStyle> style = resolvedStyle;

    element.setIsInsideRegion(false);

    if (!shouldCreateRenderer(element, renderingParentNode) && !elementInsideRegionNeedsRenderer(element, renderingParentNode, style))
        return;

    if (!style)
        style = element.styleForRenderer();

    RenderNamedFlowThread* parentFlowRenderer = 0;
#if ENABLE(CSS_REGIONS)
    parentFlowRenderer = moveToFlowThreadIfNeeded(element, *style);
#endif

    if (!element.rendererIsNeeded(*style))
        return;

    RenderElement* parentRenderer;
    RenderObject* nextRenderer;
    if (parentFlowRenderer) {
        parentRenderer = parentFlowRenderer;
        nextRenderer = parentFlowRenderer->nextRendererForElement(element);
    } else {
        // FIXME: Make this path Element only, handle the root special case separately.
        parentRenderer = renderingParentNode->renderer();
        nextRenderer = nextSiblingRenderer(element, renderingParentNode);
    }

    RenderElement* newRenderer = element.createElementRenderer(style.releaseNonNull()).leakPtr();
    if (!newRenderer)
        return;
    if (!parentRenderer->isChildAllowed(*newRenderer, newRenderer->style())) {
        newRenderer->destroy();
        return;
    }

    // Make sure the RenderObject already knows it is going to be added to a RenderFlowThread before we set the style
    // for the first time. Otherwise code using inRenderFlowThread() in the styleWillChange and styleDidChange will fail.
    newRenderer->setFlowThreadState(parentRenderer->flowThreadState());

    // Code below updateAnimations() can depend on Element::renderer() already being set.
    element.setRenderer(newRenderer);

    // FIXME: There's probably a better way to factor this.
    // This just does what setAnimatedStyle() does, except with setStyleInternal() instead of setStyle().
    newRenderer->setStyleInternal(newRenderer->animation().updateAnimations(*newRenderer, newRenderer->style()));

    newRenderer->initializeStyle();

#if ENABLE(FULLSCREEN_API)
    Document& document = element.document();
    if (document.webkitIsFullScreen() && document.webkitCurrentFullScreenElement() == &element) {
        newRenderer = RenderFullScreen::wrapRenderer(newRenderer, parentRenderer, document);
        if (!newRenderer)
            return;
    }
#endif
    // Note: Adding newRenderer instead of renderer(). renderer() may be a child of newRenderer.
    parentRenderer->addChild(newRenderer, nextRenderer);
}

static RenderObject* previousSiblingRenderer(const Text& textNode)
{
    if (textNode.renderer())
        return textNode.renderer()->previousSibling();
    for (Node* sibling = NodeRenderingTraversal::previousSibling(&textNode); sibling; sibling = NodeRenderingTraversal::previousSibling(sibling)) {
        RenderObject* renderer = sibling->renderer();
        if (renderer && !isRendererReparented(renderer))
            return renderer;
    }
    return 0;
}

static RenderObject* nextSiblingRenderer(const Text& textNode)
{
    if (textNode.renderer())
        return textNode.renderer()->nextSibling();
    for (Node* sibling = NodeRenderingTraversal::nextSibling(&textNode); sibling; sibling = NodeRenderingTraversal::nextSibling(sibling)) {
        RenderObject* renderer = sibling->renderer();
        if (renderer && !isRendererReparented(renderer))
            return renderer;
    }
    return 0;
}

static void reattachTextRenderersForWhitespaceOnlySiblingsAfterAttachIfNeeded(Node& current)
{
    if (isInsertionPoint(current))
        return;
    // This function finds sibling text renderers where the results of textRendererIsNeeded may have changed as a result of
    // the current node gaining or losing the renderer. This can only affect white space text nodes.
    for (Node* sibling = NodeRenderingTraversal::nextSibling(&current); sibling; sibling = NodeRenderingTraversal::nextSibling(sibling)) {
        // Siblings haven't been attached yet. They will be handled normally when they are.
        if (sibling->styleChangeType() == ReconstructRenderTree)
            return;
        if (sibling->isElementNode()) {
            // Text renderers beyond rendered elements can't be affected.
            if (!sibling->renderer() || isRendererReparented(sibling->renderer()))
                continue;
            return;
        }
        if (!sibling->isTextNode())
            continue;
        Text& textSibling = *toText(sibling);
        if (!textSibling.length() || !textSibling.containsOnlyWhitespace())
            return;
        Text& whitespaceTextSibling = textSibling;
        bool hadRenderer = whitespaceTextSibling.renderer();
        detachTextRenderer(whitespaceTextSibling);
        attachTextRenderer(whitespaceTextSibling);
        // No changes, futher renderers can't be affected.
        if (hadRenderer == !!whitespaceTextSibling.renderer())
            return;
    }
}

static bool textRendererIsNeeded(const Text& textNode, const RenderObject& parentRenderer, const RenderStyle& style)
{
    if (textNode.isEditingText())
        return true;
    if (!textNode.length())
        return false;
    if (style.display() == NONE)
        return false;
    if (!textNode.containsOnlyWhitespace())
        return true;
    // This text node has nothing but white space. We may still need a renderer in some cases.
    if (parentRenderer.isTable() || parentRenderer.isTableRow() || parentRenderer.isTableSection() || parentRenderer.isRenderTableCol() || parentRenderer.isFrameSet())
        return false;
    if (style.preserveNewline()) // pre/pre-wrap/pre-line always make renderers.
        return true;

    RenderObject* previousRenderer = previousSiblingRenderer(textNode);
    if (previousRenderer && previousRenderer->isBR()) // <span><br/> <br/></span>
        return false;
        
    if (parentRenderer.isRenderInline()) {
        // <span><div/> <div/></span>
        if (previousRenderer && !previousRenderer->isInline())
            return false;
    } else {
        if (parentRenderer.isRenderBlock() && !parentRenderer.childrenInline() && (!previousRenderer || !previousRenderer->isInline()))
            return false;
        
        RenderObject* first = toRenderElement(parentRenderer).firstChild();
        while (first && first->isFloatingOrOutOfFlowPositioned())
            first = first->nextSibling();
        RenderObject* nextRenderer = nextSiblingRenderer(textNode);
        if (!first || nextRenderer == first) {
            // Whitespace at the start of a block just goes away. Don't even make a render object for this text.
            return false;
        }
    }
    return true;
}

static void createTextRendererIfNeeded(Text& textNode)
{
    ASSERT(!textNode.renderer());

    ContainerNode* renderingParentNode = NodeRenderingTraversal::parent(&textNode);
    if (!renderingParentNode)
        return;
    RenderElement* parentRenderer = renderingParentNode->renderer();
    if (!parentRenderer || !parentRenderer->canHaveChildren())
        return;
    if (!renderingParentNode->childShouldCreateRenderer(textNode))
        return;

    const auto& style = parentRenderer->style();

    if (!textRendererIsNeeded(textNode, *parentRenderer, style))
        return;

    auto newRenderer = textNode.createTextRenderer(style);
    ASSERT(newRenderer);

    if (!parentRenderer->isChildAllowed(*newRenderer, style))
        return;

    // Make sure the RenderObject already knows it is going to be added to a RenderFlowThread before we set the style
    // for the first time. Otherwise code using inRenderFlowThread() in the styleWillChange and styleDidChange will fail.
    newRenderer->setFlowThreadState(parentRenderer->flowThreadState());

    RenderObject* nextRenderer = nextSiblingRenderer(textNode);
    textNode.setRenderer(newRenderer.get());
    // Parent takes care of the animations, no need to call setAnimatableStyle.
    parentRenderer->addChild(newRenderer.leakPtr(), nextRenderer);
}

void attachTextRenderer(Text& textNode)
{
    createTextRendererIfNeeded(textNode);

    textNode.clearNeedsStyleRecalc();
}

void detachTextRenderer(Text& textNode)
{
    if (textNode.renderer())
        textNode.renderer()->destroyAndCleanupAnonymousWrappers();
    textNode.setRenderer(0);
}

void updateTextRendererAfterContentChange(Text& textNode, unsigned offsetOfReplacedData, unsigned lengthOfReplacedData)
{
    RenderText* textRenderer = textNode.renderer();
    if (!textRenderer) {
        attachTextRenderer(textNode);
        reattachTextRenderersForWhitespaceOnlySiblingsAfterAttachIfNeeded(textNode);
        return;
    }
    RenderObject* parentRenderer = NodeRenderingTraversal::parent(&textNode)->renderer();
    if (!textRendererIsNeeded(textNode, *parentRenderer, textRenderer->style())) {
        detachTextRenderer(textNode);
        attachTextRenderer(textNode);
        reattachTextRenderersForWhitespaceOnlySiblingsAfterAttachIfNeeded(textNode);
        return;
    }
    textRenderer->setTextWithOffset(textNode.dataImpl(), offsetOfReplacedData, lengthOfReplacedData);
}

static void attachDistributedChildren(InsertionPoint& insertionPoint)
{
    if (ShadowRoot* shadowRoot = insertionPoint.containingShadowRoot())
        ContentDistributor::ensureDistribution(shadowRoot);
    for (Node* current = insertionPoint.firstDistributed(); current; current = insertionPoint.nextDistributedTo(current)) {
        if (current->isTextNode()) {
            if (current->renderer())
                continue;
            attachTextRenderer(*toText(current));
            continue;
        }
        if (current->isElementNode()) {
            if (current->renderer())
                detachRenderTree(*toElement(current));
            attachRenderTree(*toElement(current), nullptr);
        }
    }
}

static void attachChildren(ContainerNode& current)
{
    if (isInsertionPoint(current))
        attachDistributedChildren(toInsertionPoint(current));

    for (Node* child = current.firstChild(); child; child = child->nextSibling()) {
        ASSERT((!child->renderer() || child->inNamedFlow()) || current.shadowRoot() || isInsertionPoint(current));
        if (child->renderer())
            continue;
        if (child->isTextNode()) {
            attachTextRenderer(*toText(child));
            continue;
        }
        if (child->isElementNode())
            attachRenderTree(*toElement(child), nullptr);
    }
}

static void attachShadowRoot(ShadowRoot& shadowRoot)
{
    attachChildren(shadowRoot);

    shadowRoot.clearNeedsStyleRecalc();
    shadowRoot.clearChildNeedsStyleRecalc();
}

static PseudoElement* beforeOrAfterPseudoElement(Element& current, PseudoId pseudoId)
{
    ASSERT(pseudoId == BEFORE || pseudoId == AFTER);
    if (pseudoId == BEFORE)
        return current.beforePseudoElement();
    return current.afterPseudoElement();
}

static void setBeforeOrAfterPseudoElement(Element& current, PassRefPtr<PseudoElement> pseudoElement, PseudoId pseudoId)
{
    ASSERT(pseudoId == BEFORE || pseudoId == AFTER);
    if (pseudoId == BEFORE) {
        current.setBeforePseudoElement(pseudoElement);
        return;
    }
    current.setAfterPseudoElement(pseudoElement);
}

static void clearBeforeOrAfterPseudoElement(Element& current, PseudoId pseudoId)
{
    ASSERT(pseudoId == BEFORE || pseudoId == AFTER);
    if (pseudoId == BEFORE) {
        current.clearBeforePseudoElement();
        return;
    }
    current.clearAfterPseudoElement();
}

static bool needsPseudeElement(Element& current, PseudoId pseudoId)
{
    if (!current.document().styleSheetCollection().usesBeforeAfterRules())
        return false;
    if (!current.renderer() || !current.renderer()->canHaveGeneratedChildren())
        return false;
    if (current.isPseudoElement())
        return false;
    if (!pseudoElementRendererIsNeeded(current.renderer()->getCachedPseudoStyle(pseudoId)))
        return false;
    return true;
}

static void attachBeforeOrAfterPseudoElementIfNeeded(Element& current, PseudoId pseudoId)
{
    if (!needsPseudeElement(current, pseudoId))
        return;
    RefPtr<PseudoElement> pseudoElement = PseudoElement::create(current, pseudoId);
    setBeforeOrAfterPseudoElement(current, pseudoElement, pseudoId);
    attachRenderTree(*pseudoElement, nullptr);
}

static void attachRenderTree(Element& current, PassRefPtr<RenderStyle> resolvedStyle)
{
    PostAttachCallbackDisabler callbackDisabler(current.document());
    WidgetHierarchyUpdatesSuspensionScope suspendWidgetHierarchyUpdates;

    if (current.hasCustomStyleResolveCallbacks())
        current.willAttachRenderers();

    createRendererIfNeeded(current, resolvedStyle);

    if (current.parentElement() && current.parentElement()->isInCanvasSubtree())
        current.setIsInCanvasSubtree(true);

    attachBeforeOrAfterPseudoElementIfNeeded(current, BEFORE);

    StyleResolverParentPusher parentPusher(&current);

    // When a shadow root exists, it does the work of attaching the children.
    if (ShadowRoot* shadowRoot = current.shadowRoot()) {
        parentPusher.push();
        attachShadowRoot(*shadowRoot);
    } else if (current.firstChild())
        parentPusher.push();

    attachChildren(current);

    current.clearNeedsStyleRecalc();
    current.clearChildNeedsStyleRecalc();

    if (AXObjectCache* cache = current.document().axObjectCache())
        cache->updateCacheAfterNodeIsAttached(&current);

    attachBeforeOrAfterPseudoElementIfNeeded(current, AFTER);

    current.updateFocusAppearanceAfterAttachIfNeeded();
    
    if (current.hasCustomStyleResolveCallbacks())
        current.didAttachRenderers();
}

static void detachDistributedChildren(InsertionPoint& insertionPoint)
{
    for (Node* current = insertionPoint.firstDistributed(); current; current = insertionPoint.nextDistributedTo(current)) {
        if (current->isTextNode()) {
            detachTextRenderer(*toText(current));
            continue;
        }
        if (current->isElementNode())
            detachRenderTree(*toElement(current));
    }
}

static void detachChildren(ContainerNode& current, DetachType detachType)
{
    if (isInsertionPoint(current))
        detachDistributedChildren(toInsertionPoint(current));

    for (Node* child = current.firstChild(); child; child = child->nextSibling()) {
        if (child->isTextNode()) {
            Style::detachTextRenderer(*toText(child));
            continue;
        }
        if (child->isElementNode())
            detachRenderTree(*toElement(child), detachType);
    }
    current.clearChildNeedsStyleRecalc();
}

static void detachShadowRoot(ShadowRoot& shadowRoot, DetachType detachType)
{
    detachChildren(shadowRoot, detachType);
}

static void detachRenderTree(Element& current, DetachType detachType)
{
    WidgetHierarchyUpdatesSuspensionScope suspendWidgetHierarchyUpdates;

    if (current.hasCustomStyleResolveCallbacks())
        current.willDetachRenderers();

    current.clearStyleDerivedDataBeforeDetachingRenderer();

    // Do not remove the element's hovered and active status
    // if performing a reattach.
    if (detachType != ReattachDetach)
        current.clearHoverAndActiveStatusBeforeDetachingRenderer();

    if (ShadowRoot* shadowRoot = current.shadowRoot())
        detachShadowRoot(*shadowRoot, detachType);

    detachChildren(current, detachType);

    if (current.renderer())
        current.renderer()->destroyAndCleanupAnonymousWrappers();
    current.setRenderer(0);

    if (current.hasCustomStyleResolveCallbacks())
        current.didDetachRenderers();
}

static bool pseudoStyleCacheIsInvalid(RenderElement* renderer, RenderStyle* newStyle)
{
    const RenderStyle& currentStyle = renderer->style();

    const PseudoStyleCache* pseudoStyleCache = currentStyle.cachedPseudoStyles();
    if (!pseudoStyleCache)
        return false;

    size_t cacheSize = pseudoStyleCache->size();
    for (size_t i = 0; i < cacheSize; ++i) {
        RefPtr<RenderStyle> newPseudoStyle;
        PseudoId pseudoId = pseudoStyleCache->at(i)->styleType();
        if (pseudoId == FIRST_LINE || pseudoId == FIRST_LINE_INHERITED)
            newPseudoStyle = renderer->uncachedFirstLineStyle(newStyle);
        else
            newPseudoStyle = renderer->getUncachedPseudoStyle(PseudoStyleRequest(pseudoId), newStyle, newStyle);
        if (!newPseudoStyle)
            return true;
        if (*newPseudoStyle != *pseudoStyleCache->at(i)) {
            if (pseudoId < FIRST_INTERNAL_PSEUDOID)
                newStyle->setHasPseudoStyle(pseudoId);
            newStyle->addCachedPseudoStyle(newPseudoStyle);
            if (pseudoId == FIRST_LINE || pseudoId == FIRST_LINE_INHERITED) {
                // FIXME: We should do an actual diff to determine whether a repaint vs. layout
                // is needed, but for now just assume a layout will be required. The diff code
                // in RenderObject::setStyle would need to be factored out so that it could be reused.
                renderer->setNeedsLayoutAndPrefWidthsRecalc();
            }
            return true;
        }
    }
    return false;
}

static Change resolveLocal(Element& current, Change inheritedChange)
{
    Change localChange = Detach;
    RefPtr<RenderStyle> newStyle;
    RefPtr<RenderStyle> currentStyle = current.renderStyle();

    Document& document = current.document();
    if (currentStyle && current.styleChangeType() != ReconstructRenderTree) {
        newStyle = current.styleForRenderer();
        localChange = determineChange(currentStyle.get(), newStyle.get());
    }
    if (localChange == Detach) {
        if (current.renderer() || current.inNamedFlow())
            detachRenderTree(current, ReattachDetach);
        attachRenderTree(current, newStyle.release());
        reattachTextRenderersForWhitespaceOnlySiblingsAfterAttachIfNeeded(current);

        return Detach;
    }

    if (RenderElement* renderer = current.renderer()) {
        if (localChange != NoChange || pseudoStyleCacheIsInvalid(renderer, newStyle.get()) || (inheritedChange == Force && renderer->requiresForcedStyleRecalcPropagation()) || current.styleChangeType() == SyntheticStyleChange)
            renderer->setAnimatableStyle(*newStyle);
        else if (current.needsStyleRecalc()) {
            // Although no change occurred, we use the new style so that the cousin style sharing code won't get
            // fooled into believing this style is the same.
            renderer->setStyleInternal(*newStyle);
        }
    }

    // If "rem" units are used anywhere in the document, and if the document element's font size changes, then go ahead and force font updating
    // all the way down the tree. This is simpler than having to maintain a cache of objects (and such font size changes should be rare anyway).
    if (document.styleSheetCollection().usesRemUnits() && document.documentElement() == &current && localChange != NoChange && currentStyle && newStyle && currentStyle->fontSize() != newStyle->fontSize()) {
        // Cached RenderStyles may depend on the re units.
        if (StyleResolver* styleResolver = document.styleResolverIfExists())
            styleResolver->invalidateMatchedPropertiesCache();
        return Force;
    }
    if (inheritedChange == Force)
        return Force;
    if (current.styleChangeType() >= FullStyleChange)
        return Force;

    return localChange;
}

static void updateTextStyle(Text& text)
{
    RenderText* renderer = text.renderer();

    if (!text.needsStyleRecalc())
        return;
    if (renderer)
        renderer->setText(text.dataImpl());
    else {
        attachTextRenderer(text);
        reattachTextRenderersForWhitespaceOnlySiblingsAfterAttachIfNeeded(text);
    }
    text.clearNeedsStyleRecalc();
}

static void resolveShadowTree(ShadowRoot* shadowRoot, Style::Change change)
{
    if (!shadowRoot)
        return;

    for (Node* child = shadowRoot->firstChild(); child; child = child->nextSibling()) {
        if (child->isTextNode()) {
            updateTextStyle(*toText(child));
            continue;
        }
        if (child->isElementNode())
            resolveTree(*toElement(child), change);
    }

    shadowRoot->clearNeedsStyleRecalc();
    shadowRoot->clearChildNeedsStyleRecalc();
}

static void updateBeforeOrAfterPseudoElement(Element& current, Change change, PseudoId pseudoId)
{
    if (PseudoElement* existingPseudoElement = beforeOrAfterPseudoElement(current, pseudoId)) {
        if (needsPseudeElement(current, pseudoId))
            resolveTree(*existingPseudoElement, current.needsStyleRecalc() ? Force : change);
        else
            clearBeforeOrAfterPseudoElement(current, pseudoId);
        return;
    }
    attachBeforeOrAfterPseudoElementIfNeeded(current, pseudoId);
}

#if PLATFORM(IOS)
static EVisibility elementImplicitVisibility(const Element* element)
{
    RenderObject* renderer = element->renderer();
    if (!renderer)
        return VISIBLE;

    RenderStyle& style = renderer->style();

    Length width(style.width());
    Length height(style.height());
    if ((width.isFixed() && width.value() <= 0) || (height.isFixed() && height.value() <= 0))
        return HIDDEN;

    Length top(style.top());
    Length left(style.left());
    if (left.isFixed() && width.isFixed() && -left.value() >= width.value())
        return HIDDEN;

    if (top.isFixed() && height.isFixed() && -top.value() >= height.value())
        return HIDDEN;
    return VISIBLE;
}

class CheckForVisibilityChangeOnRecalcStyle {
public:
    CheckForVisibilityChangeOnRecalcStyle(Element* element, RenderStyle* currentStyle)
        : m_element(element)
        , m_previousDisplay(currentStyle ? currentStyle->display() : NONE)
        , m_previousVisibility(currentStyle ? currentStyle->visibility() : HIDDEN)
        , m_previousImplicitVisibility(WKObservingContentChanges() && WKContentChange() != WKContentVisibilityChange ? elementImplicitVisibility(element) : VISIBLE)
    {
    }
    ~CheckForVisibilityChangeOnRecalcStyle()
    {
        if (!WKObservingContentChanges())
            return;
        RenderStyle* style = m_element->renderStyle();
        if (!style)
            return;
        if ((m_previousDisplay == NONE && style->display() != NONE) || (m_previousVisibility == HIDDEN && style->visibility() != HIDDEN)
            || (m_previousImplicitVisibility == HIDDEN && elementImplicitVisibility(m_element.get()) == VISIBLE))
            WKSetObservedContentChange(WKContentVisibilityChange);
    }
private:
    RefPtr<Element> m_element;
    EDisplay m_previousDisplay;
    EVisibility m_previousVisibility;
    EVisibility m_previousImplicitVisibility;
};
#endif // PLATFORM(IOS)

void resolveTree(Element& current, Change change)
{
    ASSERT(change != Detach);

    if (current.hasCustomStyleResolveCallbacks()) {
        if (!current.willRecalcStyle(change))
            return;
    }

    ContainerNode* renderingParentNode = NodeRenderingTraversal::parent(&current);
    bool hasParentStyle = renderingParentNode && renderingParentNode->renderStyle();
    bool hasDirectAdjacentRules = current.childrenAffectedByDirectAdjacentRules();
    bool hasIndirectAdjacentRules = current.childrenAffectedByForwardPositionalRules();

#if PLATFORM(IOS)
    CheckForVisibilityChangeOnRecalcStyle checkForVisibilityChange(&current, current.renderStyle());
#endif

    if (change > NoChange || current.needsStyleRecalc())
        current.resetComputedStyle();

    if (hasParentStyle && (change >= Inherit || current.needsStyleRecalc()))
        change = resolveLocal(current, change);

    if (change != Detach) {
        StyleResolverParentPusher parentPusher(&current);

        if (ShadowRoot* shadowRoot = current.shadowRoot()) {
            if (change >= Inherit || shadowRoot->childNeedsStyleRecalc() || shadowRoot->needsStyleRecalc()) {
                parentPusher.push();
                resolveShadowTree(shadowRoot, change);
            }
        }

        updateBeforeOrAfterPseudoElement(current, change, BEFORE);

        // FIXME: This check is good enough for :hover + foo, but it is not good enough for :hover + foo + bar.
        // For now we will just worry about the common case, since it's a lot trickier to get the second case right
        // without doing way too much re-resolution.
        bool forceCheckOfNextElementSibling = false;
        bool forceCheckOfAnyElementSibling = false;
        for (Node* child = current.firstChild(); child; child = child->nextSibling()) {
            if (child->isTextNode()) {
                updateTextStyle(*toText(child));
                continue;
            }
            if (!child->isElementNode())
                continue;
            Element* childElement = toElement(child);
            bool childRulesChanged = childElement->needsStyleRecalc() && childElement->styleChangeType() == FullStyleChange;
            if ((forceCheckOfNextElementSibling || forceCheckOfAnyElementSibling))
                childElement->setNeedsStyleRecalc();
            if (change >= Inherit || childElement->childNeedsStyleRecalc() || childElement->needsStyleRecalc()) {
                parentPusher.push();
                resolveTree(*childElement, change);
            }
            forceCheckOfNextElementSibling = childRulesChanged && hasDirectAdjacentRules;
            forceCheckOfAnyElementSibling = forceCheckOfAnyElementSibling || (childRulesChanged && hasIndirectAdjacentRules);
        }

        updateBeforeOrAfterPseudoElement(current, change, AFTER);
    }

    current.clearNeedsStyleRecalc();
    current.clearChildNeedsStyleRecalc();
    
    if (current.hasCustomStyleResolveCallbacks())
        current.didRecalcStyle(change);
}

void resolveTree(Document& document, Change change)
{
    if (change == Force) {
        auto documentStyle = resolveForDocument(document);

        // Inserting the pictograph font at the end of the font fallback list is done by the
        // font selector, so set a font selector if needed.
        if (Settings* settings = document.settings()) {
            StyleResolver* styleResolver = document.styleResolverIfExists();
            if (settings->fontFallbackPrefersPictographs() && styleResolver)
                documentStyle.get().font().update(styleResolver->fontSelector());
        }

        Style::Change documentChange = determineChange(&documentStyle.get(), &document.renderView()->style());
        if (documentChange != NoChange)
            document.renderView()->setStyle(std::move(documentStyle));
        else
            documentStyle.dropRef();
    }

    Element* documentElement = document.documentElement();
    if (!documentElement)
        return;
    if (change < Inherit && !documentElement->childNeedsStyleRecalc() && !documentElement->needsStyleRecalc())
        return;
    resolveTree(*documentElement, change);
}

void detachRenderTree(Element& element)
{
    detachRenderTree(element, NormalDetach);
}

}
}
