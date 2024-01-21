#pragma once
// #include "atom/core.h"
#include "atom/memory/lockable.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// null_lockable is a stateless object that does not has any locking mechanism.
    /// it's used where a lockable implementation is needed but thread-safety is not needed.
    ///
    /// @todo: should we delete its constructors and operators to match {simple_mutex}?
    /// --------------------------------------------------------------------------------------------
    class null_lockable
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// default_constructor. does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr null_lockable() {}

        /// ----------------------------------------------------------------------------------------
        /// copy_constructor is default.
        /// ----------------------------------------------------------------------------------------
        constexpr null_lockable(const null_lockable& other) {}

        /// ----------------------------------------------------------------------------------------
        /// move_constructor is default.
        /// ----------------------------------------------------------------------------------------
        constexpr null_lockable(null_lockable&& other) {}

        /// ----------------------------------------------------------------------------------------
        /// copy_operator is default.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const null_lockable& other) -> null_lockable&
        {
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// move_operator is default.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(null_lockable&& other) -> null_lockable&
        {
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// destructor. does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr ~null_lockable() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr void lock() {}

        /// ----------------------------------------------------------------------------------------
        /// always returns true.
        /// ----------------------------------------------------------------------------------------
        constexpr bool try_lock()
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr void unlock() {}
    };

    static_assert(rlockable<null_lockable>);

    /// --------------------------------------------------------------------------------------------
    /// specialization for null_lockable to avoid any performance overhead.
    /// --------------------------------------------------------------------------------------------
    template <>
    class lock_guard<null_lockable>
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// constructor. does nothing.
        ///
        /// @param[in] lock null_lockable reference. (unused).
        /// ----------------------------------------------------------------------------------------
        constexpr lock_guard(null_lockable& lock) {}

        /// ----------------------------------------------------------------------------------------
        /// destructor. does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr ~lock_guard() {}
    };
}
