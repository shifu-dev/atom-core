#pragma once
#include "ArrIter.h"
#include "InitList.h"
#include "RangeReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... TArgs>
    class Range;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TIter_, typename TIterEnd_>
    requires(RIterPair<TIter_, TIterEnd_>)
    class Range<TIter_, TIterEnd_>
    {
    public:
        using TElem = typename TIter_::TElem;
        using TMutIter = TIter_;
        using TMutIterEnd = TIterEnd_;
        using TIter = TIter_;
        using TIterEnd = TIterEnd_;

    public:
        constexpr Range(TIter iter, TIterEnd end)
            : _iter{ iter }
            , _iterEnd{ end }
        {}

    public:
        constexpr auto iter() const -> TIter
            requires(RIterPair<TIter, TIterEnd>)
        {
            return _iter;
        }

        constexpr auto iterEnd() const -> TIterEnd
            requires(RIterPair<TIter, TIterEnd>)
        {
            return _iterEnd;
        }

        constexpr auto mutIter() -> TMutIter
            requires(RMutIterPair<TMutIter, TMutIterEnd>)
        {
            return _iter;
        }

        constexpr auto mutIterEnd() -> TMutIterEnd
            requires(RMutIterPair<TMutIter, TMutIterEnd>)
        {
            return _iterEnd;
        }

        constexpr auto count() const -> usize
            requires(RJumpIterPair<TIter, TIterEnd>)
        {
            return -_iter.compare(_iterEnd);
        }

        constexpr auto data() const -> const TElem*
            requires(RArrIterPair<TIter, TIterEnd>)
        {
            return _iter.data();
        }

        constexpr auto data() -> TElem*
            requires(RMutArrIterPair<TIter, TIterEnd>)
        {
            return _iter.data();
        }

    protected:
        TIter _iter;
        TIterEnd _iterEnd;
    };

    template <typename TIter, typename TIterEnd>
        requires(RIterPair<TIter, TIterEnd>)
    Range(TIter iter, TIterEnd end) -> Range<TIter, TIterEnd>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class Range<const T*, const T*>: public Range<ArrIter<T>, ArrIter<T>>
    {
    public:
        using Base = Range<ArrIter<T>, ArrIter<T>>;

    public:
        constexpr Range(const T* arr, const T* end)
            : Base{ arr, end }
        {}

        constexpr Range(const T* arr, usize count)
            : Base{ arr, arr + count }
        {}

        template <usize count>
        constexpr Range(const T (&arr)[count])
            : Base{ arr, arr + count }
        {}
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class Range<T*, T*>: public Range<MutArrIter<T>, MutArrIter<T>>
    {
    public:
        using Base = Range<MutArrIter<T>, MutArrIter<T>>;

    public:
        constexpr Range(T* arr, T* end)
            : Base{ arr, end }
        {}

        constexpr Range(T* arr, usize count)
            : Base{ arr, arr + count }
        {}

        template <usize count>
        constexpr Range(T (&arr)[count])
            : Base{ arr, arr + count }
        {}
    };

    template <typename T>
    Range(T* iter, T* end) -> Range<T*, T*>;

    template <typename T>
    Range(T* iter, usize count) -> Range<T*, T*>;

    template <typename T, usize count>
    Range(T (&arr)[count]) -> Range<T*, T*>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class Range<InitList<T>>: public Range<const T*, const T*>
    {
    public:
        using Base = Range<const T*, const T*>;

    public:
        constexpr Range(const InitList<T>& init)
            : Base{ init.begin(), init.end() }
        {}
    };

    template <typename T>
    Range(const InitList<T>& init) -> Range<InitList<T>>;
}