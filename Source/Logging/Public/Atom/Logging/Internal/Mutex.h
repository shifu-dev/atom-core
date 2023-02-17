#pragma once
#include <mutex>
#include <concepts>

#define STATIC_ASSERT_MSG(assertion, msg) static_assert(assertion, msg)
#define STATIC_ASSERT(assertion) static_assert(assertion)
#define STATIC_THROW(msg) static_assert(false, msg)

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
//// Lockable Requirements.
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Requirements for Lockable type.
    /// --------------------------------------------------------------------------------------------
    template <typename LockableType>
    concept LockableRequirements = requires(LockableType lock)
    {
        { lock.Lock() } -> std::same_as<void>;
        { lock.TryLock() } -> std::same_as<bool>;
        { lock.Unlock() } -> std::same_as<void>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Alias for LockableRequirements to use inline with auto.
    /// --------------------------------------------------------------------------------------------
    template <typename LockableType>
    concept Lockable = LockableRequirements<LockableType>;

//// -----------------------------------------------------------------------------------------------
//// Lockable Implementations.
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// NullLockable is a stateless object that doesn't has any locking mechanism.
    /// It's used where a Lockable implementation is needed but thread-safety is not needed.
    /// --------------------------------------------------------------------------------------------
    class NullLockable
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor. Does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr NullLockable() noexcept { }

        /// ----------------------------------------------------------------------------------------
        /// CopyConstructor is deleted.
        /// ----------------------------------------------------------------------------------------
        constexpr NullLockable(const NullLockable& other) = delete;

        /// ----------------------------------------------------------------------------------------
        /// MoveConstructor is default.
        /// ----------------------------------------------------------------------------------------
        constexpr NullLockable(NullLockable&& other) = default;

        /// ----------------------------------------------------------------------------------------
        /// CopyOperator is deleted.
        /// ----------------------------------------------------------------------------------------
        constexpr NullLockable& operator = (const NullLockable& other) = delete;

        /// ----------------------------------------------------------------------------------------
        /// MoveOperator is default.
        /// ----------------------------------------------------------------------------------------
        constexpr NullLockable& operator = (NullLockable&& other) = default;

        /// ----------------------------------------------------------------------------------------
        /// Destructor. Does nothing.
        /// ----------------------------------------------------------------------------------------
        ~NullLockable() { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr void Lock() { }

        /// ----------------------------------------------------------------------------------------
        /// Always returns true.
        /// ----------------------------------------------------------------------------------------
        constexpr bool TryLock() { return true; }

        /// ----------------------------------------------------------------------------------------
        /// Does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr void Unlock() { }
    };

    STATIC_ASSERT(Lockable<NullLockable>);

    /// --------------------------------------------------------------------------------------------
    /// SimpleMutex implementation.
    /// 
    /// @TODO Implement this class without {std::lock}.
    /// --------------------------------------------------------------------------------------------
    class SimpleMutex
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// 
        /// @POST Mutex is not locked.
        /// ----------------------------------------------------------------------------------------
        constexpr SimpleMutex() noexcept { }

        /// ----------------------------------------------------------------------------------------
        /// CopyConstructor is deleted.
        /// ----------------------------------------------------------------------------------------
        SimpleMutex(const SimpleMutex& other) = delete;

        /// ----------------------------------------------------------------------------------------
        /// MoveConstructor is default.
        /// ----------------------------------------------------------------------------------------
        SimpleMutex(SimpleMutex&& other) = default;

        /// ----------------------------------------------------------------------------------------
        /// CopyOperator is deleted.
        /// ----------------------------------------------------------------------------------------
        SimpleMutex& operator = (const SimpleMutex& other) = delete;

        /// ----------------------------------------------------------------------------------------
        /// MoveOperator is default.
        /// ----------------------------------------------------------------------------------------
        SimpleMutex& operator = (SimpleMutex&& other) = default;

        /// ----------------------------------------------------------------------------------------
        /// Destructor.
        /// 
        /// @NOTE If lock is locked by some thread and lock is destroyed, behaviour is undefined.
        /// ----------------------------------------------------------------------------------------
        ~SimpleMutex() { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Locks the lock. If the lock is already locked by some thread then blocks the calling
        /// thread until lock is acquired.
        /// 
        /// @SEE TryLock().
        /// ----------------------------------------------------------------------------------------
        void Lock()
        {
            _impl.lock();
        }

        /// ----------------------------------------------------------------------------------------
        /// Tries to lock the lock. If the lock is already locked by some thread then returns but 
        /// doesn't blocks the thread.
        /// 
        /// @RETURNS {true} if lock acquired, else {false}.
        /// ----------------------------------------------------------------------------------------
        bool TryLock()
        {
            return _impl.try_lock();
        }

        /// ----------------------------------------------------------------------------------------
        /// Unlocks the lock.
        /// ----------------------------------------------------------------------------------------
        void Unlock()
        {
            _impl.unlock();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Mutex implementation.
        /// ----------------------------------------------------------------------------------------
        std::mutex _impl;
    };

    STATIC_ASSERT(Lockable<SimpleMutex>);

//// -----------------------------------------------------------------------------------------------
//// Lock Guard.
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Locks the lock on construction and unlocks at destruction. This is done to guarantee
    /// exception safety.
    /// --------------------------------------------------------------------------------------------
    template <typename LockableType>
        requires LockableRequirements<LockableType>
    class LockGuard
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Constructor. Locks the lock.
        /// 
        /// @PARAM[IN] lock Lockable to lock.
        /// 
        /// @THROWS UnkownException Exception thrown by {lock.Lock()}.
        /// ----------------------------------------------------------------------------------------
        LockGuard(LockableType& lock):
            _lock(lock)
        {
            _lock.Lock();
        }

        /// ----------------------------------------------------------------------------------------
        /// Destructor. Unlocks the lock.
        /// 
        /// @THROWS UnkownException Exception thrown by {lock.Lock()}.
        /// ----------------------------------------------------------------------------------------
        ~LockGuard()
        {
            _lock.Unlock();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Lockable object.
        /// ----------------------------------------------------------------------------------------
        LockableType& _lock;
    };

    /// --------------------------------------------------------------------------------------------
    /// Specialization for NullLockable to avoid any performance overhead.
    /// --------------------------------------------------------------------------------------------
    template <>
    class LockGuard <NullLockable>
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Constructor. Does nothing. It's implemented to meet the LockableRequirements.
        /// 
        /// @PARAM[IN] lock NullLockable reference. (UNUSED).
        /// ----------------------------------------------------------------------------------------
        constexpr LockGuard(NullLockable& lock) noexcept { }

        /// ----------------------------------------------------------------------------------------
        /// Destructor. Does nothing.
        /// ----------------------------------------------------------------------------------------
        ~LockGuard() { }
    };
}