/*
 * Copyright (C) 2000 Peter Kelly (pmk@post.com)
 * Copyright (C) 2006, 2009 Apple Inc. All rights reserved.
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

#ifndef Notation_h
#define Notation_h

#include "ContainerNode.h"

namespace WebCore {

// FIXME: This class is never instantiated. Maybe it should be removed.

class Notation : public ContainerNode {
public:
    String publicId() const { ASSERT_NOT_REACHED(); return String(); }
    String systemId() const { ASSERT_NOT_REACHED(); return String(); }

private:
    Notation(Document& document)
        : ContainerNode(document)
    { }
};

} //namespace

#endif
