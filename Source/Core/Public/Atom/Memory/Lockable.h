#pragma once
#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Requirements for Lockable type.
    /// --------------------------------------------------------------------------------------------
    template <tname TLockable>
    concept RLockable = requires(TLockable lock)
    {
        { lock.Lock() } -> RSameAs<void>;
        { lock.TryLock() } -> RSameAs<bool>;
        { lock.Unlock() } -> RSameAs<void>;
    };
}