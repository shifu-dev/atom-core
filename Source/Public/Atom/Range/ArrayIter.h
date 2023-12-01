#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// ArrayIter iterates over raw arr.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayIter
    {
        static_assert(TTI::IsPure<T>, "ArrayIter supports only pure types.");
        static_assert(not TTI::IsVoid<T>, "ArrayIter doesn't support void.");

    public:
        using TElem = T;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIter()
            : _it{ nullptr } {}

        /// ----------------------------------------------------------------------------------------
        /// # Value Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIter(MemPtr<T> it)
            : _it{ it } {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// Access value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() const -> const T&
        {
            return _it.get();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() const -> MemPtr<T>
        {
            return this->_it;
        }

        /// ----------------------------------------------------------------------------------------
        /// Move fwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        constexpr auto next() -> ArrayIter&
        {
            _it++;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Move fwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto next(usize steps) -> ArrayIter&
        {
            _it += steps;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Move bwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev() -> ArrayIter&
        {
            _it--;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Move bwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev(usize steps) -> ArrayIter&
        {
            _it -= steps;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Check if this iter is same as `that`. Used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto eq(const ArrayIter& that) const -> bool
        {
            return this->_it == that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// Check if this iter is not same as `that`. Used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ne(const ArrayIter& that) const -> bool
        {
            return not eq(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Compares `this` data with `that` data.
        /// ----------------------------------------------------------------------------------------
        constexpr auto compare(const ArrayIter& that) const -> isize
        {
            return this->_it - that._it;
        }

    protected:
        MemPtr<T> _it;
    };

    /// --------------------------------------------------------------------------------------------
    /// MutArrayIter iterates over mut raw arr.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutArrayIter: public ArrayIter<T>
    {
        static_assert(TTI::IsPure<T>, "MutArrayIter supports only pure types.");
        static_assert(not TTI::IsVoid<T>, "MutArrayIter doesn't support void.");

    private:
        using Base = ArrayIter<T>;

    public:
        using TElem = T;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr MutArrayIter()
            : Base{ } {}

        /// ----------------------------------------------------------------------------------------
        /// # Value Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr MutArrayIter(MutMemPtr<T> it)
            : Base{ it } {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// Access value by mut ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutValue() -> T&
        {
            return const_cast<T&>(Base::value());
        }

        /// ----------------------------------------------------------------------------------------
        /// Access value by mut ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutData() -> MutMemPtr<T>
        {
            return const_cast<MutMemPtr<T>>(Base::data());
        }

        /// ----------------------------------------------------------------------------------------
        /// Move fwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        constexpr auto next() -> MutArrayIter&
        {
            Base::next();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Move fwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto next(usize steps) -> MutArrayIter&
        {
            Base::next(steps);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Move bwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev() -> MutArrayIter&
        {
            Base::prev();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Move bwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev(usize steps) -> MutArrayIter&
        {
            Base::prev(steps);
            return *this;
        }
    };
}
