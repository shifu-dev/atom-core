#pragma once
#include <vector>

#include "Atom/Containers/ArrayIterator.h"
#include "Atom/Containers/Iterable.h"
#include "Atom/Containers/Insertable.h"

namespace Atom
{
    template <typename T>
    using DefaultAllocator = ::std::allocator<T>;

    template <typename T, typename TAllocator = DefaultAllocator<T>>
    class DynamicArray
    {
        using STD_TVector = ::std::vector<T>;

    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

    public:
        using TIterator = ArrayIterator<T>;

    //// -------------------------------------------------------------------------------------------
    //// Constructors, Operators and Destructor
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray() noexcept:
            _vector{ } { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterator<T> TInput>
        constexpr DynamicArray(TInput in)
        {
            InsertBack(MOVE(in));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(const STD_TVector& vec) noexcept:
            _vector{ vec } { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(STD_TVector&& vec) noexcept:
            _vector{ MOVE(vec) } { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator operator += (const T& in)
        {
            return InsertBack(in);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator operator += (T&& in)
        {
            return InsertBack(MOVE(in));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterator<T> TInput>
        constexpr TIterator operator += (TInput in)
        {
            return InsertBack(MOVE(in));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ~DynamicArray() { }

    //// -------------------------------------------------------------------------------------------
    //// Access
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr T* Data() noexcept
        {
            return _vector.data();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr const T* Data() const noexcept
        {
            return _vector.data();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr usize Count() const noexcept
        {
            return _vector.size();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool IsEmpty() const noexcept
        {
            return Count() == 0;
        }

    //// -------------------------------------------------------------------------------------------
    //// Iterator
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator Iterator() noexcept
        {
            return TIterator(_vector.data(), _vector.size());
        }

    //// -------------------------------------------------------------------------------------------
    //// Insert
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator Insert(TIterator pos, const T& in)
        {
            usize index = _vector.data() - pos.begin();
            auto out = _vector.insert(_vector.begin() + index, in);
            return TIterator{ _vector.data() + index, Count() - index };
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator Insert(TIterator pos, T&& in)
        {
            usize index = _vector.data() - pos.begin();
            auto out = _vector.insert(_vector.begin() + index, MOVE(in));
            return TIterator{ _vector.data() + index, Count() - index };
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterator<T> TInput>
        constexpr TIterator Insert(TIterator pos, TInput in)
        {
            return TIterator{ };
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertFront(const T& in)
        {
            return Insert(Iterator(), in);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertFront(T&& in)
        {
            return Insert(Iterator(), MOVE(in));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterator<T> TInput>
        constexpr TIterator InsertFront(TInput in)
        {
            return Insert(Iterator(), MOVE(in));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertBack(const T& in)
        {
            auto it = Iterator();
            it.Next(Count());
            return Insert(it, in);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertBack(T&& in)
        {
            auto it = Iterator();
            it.Next(Count());
            return Insert(it, MOVE(in));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterator<T> TInput>
        constexpr TIterator InsertBack(TInput in)
        {
            auto it = Iterator();
            it.Next(Count());
            return Insert(it, MOVE(in));
        }

    //// -------------------------------------------------------------------------------------------
    //// Remove
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterator<T> TInput>
        constexpr usize Remove(TInput in)
        {
            usize count = 0;

            for (const auto& element : in)
            {
                for (auto it = _vector.begin(); it != _vector.end(); it++)
                {
                    if (*it == element)
                    {
                        it = _vector.erase(it);
                        count++;
                    }
                }
            }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvocable<bool, const T&> TFilter>
        constexpr usize Remove(TFilter&& filter)
        {
            usize count = 0;

            for (auto it = _vector.begin(); it != _vector.end(); it++)
            {
                if (filter(*it))
                {
                    it = _vector.erase(it);
                    count++;
                }
            }

            return count;
        }

    //// -------------------------------------------------------------------------------------------
    //// Fields
    //// -------------------------------------------------------------------------------------------

    private:
        STD_TVector _vector;
    };
}