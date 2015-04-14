/*
 * Copyright (C) 2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
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
#include "CSSParserValues.h"

#include "CSSPrimitiveValue.h"
#include "CSSFunctionValue.h"
#include "CSSSelector.h"
#include "CSSSelectorList.h"

namespace WebCore {

using namespace WTF;

void destroy(const CSSParserValue& value)
{
    if (value.unit == CSSParserValue::Function)
        delete value.function;
}

CSSParserValueList::~CSSParserValueList()
{
    for (size_t i = 0, size = m_values.size(); i < size; i++)
        destroy(m_values[i]);
}

void CSSParserValueList::addValue(const CSSParserValue& v)
{
    m_values.append(v);
}

void CSSParserValueList::insertValueAt(unsigned i, const CSSParserValue& v)
{
    m_values.insert(i, v);
}

void CSSParserValueList::deleteValueAt(unsigned i)
{
    m_values.remove(i);
}

void CSSParserValueList::extend(CSSParserValueList& valueList)
{
    for (unsigned int i = 0; i < valueList.size(); ++i)
        m_values.append(*(valueList.valueAt(i)));
}

PassRefPtr<CSSValue> CSSParserValue::createCSSValue()
{
    RefPtr<CSSValue> parsedValue;
    if (id)
        return CSSPrimitiveValue::createIdentifier(id);
    
    if (unit == CSSParserValue::Operator) {
        RefPtr<CSSPrimitiveValue> primitiveValue = CSSPrimitiveValue::createParserOperator(iValue);
        primitiveValue->setPrimitiveType(CSSPrimitiveValue::CSS_PARSER_OPERATOR);
        return primitiveValue.release();
    }
    if (unit == CSSParserValue::Function)
        return CSSFunctionValue::create(function);
    if (unit >= CSSParserValue::Q_EMS)
        return CSSPrimitiveValue::createAllowingMarginQuirk(fValue, CSSPrimitiveValue::CSS_EMS);

    CSSPrimitiveValue::UnitTypes primitiveUnit = static_cast<CSSPrimitiveValue::UnitTypes>(unit);
    switch (primitiveUnit) {
    case CSSPrimitiveValue::CSS_IDENT:
    case CSSPrimitiveValue::CSS_PROPERTY_ID:
    case CSSPrimitiveValue::CSS_VALUE_ID:
        return CSSPrimitiveValue::create(string, CSSPrimitiveValue::CSS_PARSER_IDENTIFIER);
    case CSSPrimitiveValue::CSS_NUMBER:
        return CSSPrimitiveValue::create(fValue, isInt ? CSSPrimitiveValue::CSS_PARSER_INTEGER : CSSPrimitiveValue::CSS_NUMBER);
    case CSSPrimitiveValue::CSS_STRING:
    case CSSPrimitiveValue::CSS_URI:
    case CSSPrimitiveValue::CSS_PARSER_HEXCOLOR:
        return CSSPrimitiveValue::create(string, primitiveUnit);
    case CSSPrimitiveValue::CSS_PERCENTAGE:
    case CSSPrimitiveValue::CSS_EMS:
    case CSSPrimitiveValue::CSS_EXS:
    case CSSPrimitiveValue::CSS_PX:
    case CSSPrimitiveValue::CSS_CM:
    case CSSPrimitiveValue::CSS_MM:
    case CSSPrimitiveValue::CSS_IN:
    case CSSPrimitiveValue::CSS_PT:
    case CSSPrimitiveValue::CSS_PC:
    case CSSPrimitiveValue::CSS_DEG:
    case CSSPrimitiveValue::CSS_RAD:
    case CSSPrimitiveValue::CSS_GRAD:
    case CSSPrimitiveValue::CSS_MS:
    case CSSPrimitiveValue::CSS_S:
    case CSSPrimitiveValue::CSS_HZ:
    case CSSPrimitiveValue::CSS_KHZ:
    case CSSPrimitiveValue::CSS_VW:
    case CSSPrimitiveValue::CSS_VH:
    case CSSPrimitiveValue::CSS_VMIN:
    case CSSPrimitiveValue::CSS_VMAX:
    case CSSPrimitiveValue::CSS_TURN:
    case CSSPrimitiveValue::CSS_REMS:
    case CSSPrimitiveValue::CSS_CHS:
    case CSSPrimitiveValue::CSS_FR:
        return CSSPrimitiveValue::create(fValue, primitiveUnit);
    case CSSPrimitiveValue::CSS_UNKNOWN:
    case CSSPrimitiveValue::CSS_DIMENSION:
    case CSSPrimitiveValue::CSS_ATTR:
    case CSSPrimitiveValue::CSS_COUNTER:
    case CSSPrimitiveValue::CSS_RECT:
    case CSSPrimitiveValue::CSS_RGBCOLOR:
    case CSSPrimitiveValue::CSS_DPPX:
    case CSSPrimitiveValue::CSS_DPI:
    case CSSPrimitiveValue::CSS_DPCM:
    case CSSPrimitiveValue::CSS_PAIR:
#if ENABLE(DASHBOARD_SUPPORT)
    case CSSPrimitiveValue::CSS_DASHBOARD_REGION:
#endif
    case CSSPrimitiveValue::CSS_UNICODE_RANGE:
    case CSSPrimitiveValue::CSS_PARSER_OPERATOR:
    case CSSPrimitiveValue::CSS_PARSER_INTEGER:
    case CSSPrimitiveValue::CSS_PARSER_IDENTIFIER:
    case CSSPrimitiveValue::CSS_COUNTER_NAME:
    case CSSPrimitiveValue::CSS_SHAPE:
    case CSSPrimitiveValue::CSS_QUAD:
    case CSSPrimitiveValue::CSS_CALC:
    case CSSPrimitiveValue::CSS_CALC_PERCENTAGE_WITH_NUMBER:
    case CSSPrimitiveValue::CSS_CALC_PERCENTAGE_WITH_LENGTH:
        return 0;
    }

    ASSERT_NOT_REACHED();
    return 0;
}

CSSParserSelector::CSSParserSelector()
    : m_selector(std::make_unique<CSSSelector>())
{
}

CSSParserSelector::CSSParserSelector(const QualifiedName& tagQName)
    : m_selector(std::make_unique<CSSSelector>(tagQName))
{
}

CSSParserSelector::~CSSParserSelector()
{
    if (!m_tagHistory)
        return;
    Vector<std::unique_ptr<CSSParserSelector>, 16> toDelete;
    std::unique_ptr<CSSParserSelector> selector = std::move(m_tagHistory);
    while (true) {
        std::unique_ptr<CSSParserSelector> next = std::move(selector->m_tagHistory);
        toDelete.append(std::move(selector));
        if (!next)
            break;
        selector = std::move(next);
    }
}

void CSSParserSelector::adoptSelectorVector(Vector<std::unique_ptr<CSSParserSelector>>& selectorVector)
{
    auto selectorList = std::make_unique<CSSSelectorList>();
    selectorList->adoptSelectorVector(selectorVector);
    m_selector->setSelectorList(std::move(selectorList));
}

bool CSSParserSelector::isSimple() const
{
    if (m_selector->selectorList() || m_selector->matchesPseudoElement())
        return false;

    if (!m_tagHistory)
        return true;

    if (m_selector->m_match == CSSSelector::Tag) {
        // We can't check against anyQName() here because namespace may not be nullAtom.
        // Example:
        //     @namespace "http://www.w3.org/2000/svg";
        //     svg:not(:root) { ...
        if (m_selector->tagQName().localName() == starAtom)
            return m_tagHistory->isSimple();
    }

    return false;
}

void CSSParserSelector::insertTagHistory(CSSSelector::Relation before, std::unique_ptr<CSSParserSelector> selector, CSSSelector::Relation after)
{
    if (m_tagHistory)
        selector->setTagHistory(std::move(m_tagHistory));
    setRelation(before);
    selector->setRelation(after);
    m_tagHistory = std::move(selector);
}

void CSSParserSelector::appendTagHistory(CSSSelector::Relation relation, std::unique_ptr<CSSParserSelector> selector)
{
    CSSParserSelector* end = this;
    while (end->tagHistory())
        end = end->tagHistory();
    end->setRelation(relation);
    end->setTagHistory(std::move(selector));
}

void CSSParserSelector::prependTagSelector(const QualifiedName& tagQName, bool tagIsForNamespaceRule)
{
    auto second = std::make_unique<CSSParserSelector>();
    second->m_selector = std::move(m_selector);
    second->m_tagHistory = std::move(m_tagHistory);
    m_tagHistory = std::move(second);

    m_selector = std::make_unique<CSSSelector>(tagQName, tagIsForNamespaceRule);
    m_selector->m_relation = CSSSelector::SubSelector;
}

}

