/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010, 2013 Apple Inc. All rights reserved.
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

#ifndef RefCounted_h
#define RefCounted_h

#include <wtf/Assertions.h>
#include <wtf/FastAllocBase.h>
#include <wtf/Noncopyable.h>
#include <wtf/OwnPtr.h>
#include <wtf/ThreadRestrictionVerifier.h>

namespace WTF {

#ifdef NDEBUG
#define CHECK_REF_COUNTED_LIFECYCLE 0
#else
#define CHECK_REF_COUNTED_LIFECYCLE 1
#endif

// This base class holds the non-template methods and attributes.
// The RefCounted class inherits from it reducing the template bloat
// generated by the compiler (technique called template hoisting).
class RefCountedBase {
public:
    void ref()
    {
#if CHECK_REF_COUNTED_LIFECYCLE
        // Start thread verification as soon as the ref count gets to 2. This
        // heuristic reflects the fact that items are often created on one thread
        // and then given to another thread to be used.
        // FIXME: Make this restriction tigher. Especially as we move to more
        // common methods for sharing items across threads like CrossThreadCopier.h
        // We should be able to add a "detachFromThread" method to make this explicit.
        if (m_refCount == 1)
            m_verifier.setShared(true);
        // If this assert fires, it either indicates a thread safety issue or
        // that the verification needs to change. See ThreadRestrictionVerifier for
        // the different modes.
        ASSERT(m_verifier.isSafeToUse());
        ASSERT(!m_deletionHasBegun);
        ASSERT(!m_adoptionIsRequired);
#endif
        ++m_refCount;
    }

    bool hasOneRef() const
    {
#if CHECK_REF_COUNTED_LIFECYCLE
        ASSERT(m_verifier.isSafeToUse());
        ASSERT(!m_deletionHasBegun);
#endif
        return m_refCount == 1;
    }

    unsigned refCount() const
    {
#if CHECK_REF_COUNTED_LIFECYCLE
        ASSERT(m_verifier.isSafeToUse());
#endif
        return m_refCount;
    }

    void setMutexForVerifier(Mutex&);

#if HAVE(DISPATCH_H)
    void setDispatchQueueForVerifier(dispatch_queue_t);
#endif

    // Turns off verification. Use of this method is discouraged (instead extend
    // ThreadRestrictionVerifier to verify your case).
    // NB. It is necessary to call this in the constructor of many objects in
    // JavaScriptCore, because JavaScriptCore objects may be used from multiple
    // threads even if the reference counting is done in a racy manner. This is
    // because a JSC instance may be used from multiple threads so long as all
    // accesses into that instance are protected by a per-instance lock. It would
    // be absolutely wrong to prohibit this pattern, and it would be a disastrous
    // regression to require that the objects within that instance use a thread-
    // safe version of reference counting.
    void turnOffVerifier()
    {
#if CHECK_REF_COUNTED_LIFECYCLE
        m_verifier.turnOffVerification();
#endif
    }

    void relaxAdoptionRequirement()
    {
#if CHECK_REF_COUNTED_LIFECYCLE
        ASSERT(!m_deletionHasBegun);
        ASSERT(m_adoptionIsRequired);
        m_adoptionIsRequired = false;
#endif
    }

protected:
    RefCountedBase()
        : m_refCount(1)
#if CHECK_REF_COUNTED_LIFECYCLE
        , m_deletionHasBegun(false)
        , m_adoptionIsRequired(true)
#endif
    {
    }

    ~RefCountedBase()
    {
#if CHECK_REF_COUNTED_LIFECYCLE
        ASSERT(m_deletionHasBegun);
        ASSERT(!m_adoptionIsRequired);
#endif
    }

    // Returns whether the pointer should be freed or not.
    bool derefBase()
    {
#if CHECK_REF_COUNTED_LIFECYCLE
        ASSERT(m_verifier.isSafeToUse());
        ASSERT(!m_deletionHasBegun);
        ASSERT(!m_adoptionIsRequired);
#endif

        ASSERT(m_refCount);
        unsigned tempRefCount = m_refCount - 1;
        if (!tempRefCount) {
#if CHECK_REF_COUNTED_LIFECYCLE
            m_deletionHasBegun = true;
#endif
            return true;
        }
        m_refCount = tempRefCount;

#if CHECK_REF_COUNTED_LIFECYCLE
        // Stop thread verification when the ref goes to 1 because it
        // is safe to be passed to another thread at this point.
        if (m_refCount == 1)
            m_verifier.setShared(false);
#endif
        return false;
    }

#if CHECK_REF_COUNTED_LIFECYCLE
    bool deletionHasBegun() const
    {
        return m_deletionHasBegun;
    }
#endif

private:

#if CHECK_REF_COUNTED_LIFECYCLE
    friend void adopted(RefCountedBase*);
#endif

    unsigned m_refCount;
#if CHECK_REF_COUNTED_LIFECYCLE
    bool m_deletionHasBegun;
    bool m_adoptionIsRequired;
    ThreadRestrictionVerifier m_verifier;
#endif
};

#if CHECK_REF_COUNTED_LIFECYCLE
inline void adopted(RefCountedBase* object)
{
    if (!object)
        return;
    ASSERT(!object->m_deletionHasBegun);
    object->m_adoptionIsRequired = false;
}
#endif

template<typename T> class RefCounted : public RefCountedBase {
    WTF_MAKE_NONCOPYABLE(RefCounted); WTF_MAKE_FAST_ALLOCATED;
public:
    void deref()
    {
        if (derefBase())
            delete static_cast<T*>(this);
    }

protected:
    RefCounted() { }
    ~RefCounted()
    {
    }
};

template<typename T> class RefCountedCustomAllocated : public RefCountedBase {
    WTF_MAKE_NONCOPYABLE(RefCountedCustomAllocated);

public:
    void deref()
    {
        if (derefBase())
            delete static_cast<T*>(this);
    }

protected:
    ~RefCountedCustomAllocated()
    {
    }
};

#if CHECK_REF_COUNTED_LIFECYCLE
inline void RefCountedBase::setMutexForVerifier(Mutex& mutex)
{
    m_verifier.setMutexMode(mutex);
}
#else
inline void RefCountedBase::setMutexForVerifier(Mutex&) { }
#endif

#if HAVE(DISPATCH_H)
#if CHECK_REF_COUNTED_LIFECYCLE
inline void RefCountedBase::setDispatchQueueForVerifier(dispatch_queue_t queue)
{
    m_verifier.setDispatchQueueMode(queue);
}
#else
inline void RefCountedBase::setDispatchQueueForVerifier(dispatch_queue_t) { }
#endif
#endif // HAVE(DISPATCH_H)

} // namespace WTF

using WTF::RefCounted;
using WTF::RefCountedCustomAllocated;

#endif // RefCounted_h
