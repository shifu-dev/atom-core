export module atom.core:mutex;
import :std;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// simple_mutex implementation.
    ///
    /// @todo implement this class without {std::lock}.
    /// --------------------------------------------------------------------------------------------
    export class simple_mutex
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// default_constructor.
        ///
        /// @post mutex is not locked.
        /// ----------------------------------------------------------------------------------------
        simple_mutex() {}

        /// ----------------------------------------------------------------------------------------
        /// copy_constructor is deleted.
        /// ----------------------------------------------------------------------------------------
        simple_mutex(const simple_mutex& other) = delete;

        /// ----------------------------------------------------------------------------------------
        /// move_constructor is delete.
        /// ----------------------------------------------------------------------------------------
        simple_mutex(simple_mutex&& other) = delete;

        /// ----------------------------------------------------------------------------------------
        /// copy_operator is deleted.
        /// ----------------------------------------------------------------------------------------
        auto operator=(const simple_mutex& other) = delete;

        /// ----------------------------------------------------------------------------------------
        /// move_operator is delete.
        /// ----------------------------------------------------------------------------------------
        auto operator=(simple_mutex&& other) = delete;

        /// ----------------------------------------------------------------------------------------
        /// destructor.
        ///
        /// @note if lock is locked by some thread and lock is destroyed, behaviour is undefined.
        /// ----------------------------------------------------------------------------------------
        ~simple_mutex() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// locks the lock. if the lock is already locked by some thread then blocks the calling
        /// thread until lock is acquired.
        ///
        /// @see try_lock().
        /// ----------------------------------------------------------------------------------------
        auto lock()
        {
            _impl.lock();
        }

        /// ----------------------------------------------------------------------------------------
        /// tries to lock the lock. if the lock is already locked by some thread then returns but
        /// does not blocks the thread.
        ///
        /// @returns `true` if lock acquired, else `false`.
        /// ----------------------------------------------------------------------------------------
        auto try_lock() -> bool
        {
            return _impl.try_lock();
        }

        /// ----------------------------------------------------------------------------------------
        /// unlocks the lock.
        /// ----------------------------------------------------------------------------------------
        auto unlock()
        {
            _impl.unlock();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// mutex implementation.
        /// ----------------------------------------------------------------------------------------
        std::mutex _impl;
    };
}