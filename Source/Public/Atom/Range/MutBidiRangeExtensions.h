#pragma once
#include "BidiRangeExtensions.h"
#include "MutFwdRangeExtensions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TExtensionsImpl = void>
    class _MutBidiRangeExtensionsImpl:
        public _MutFwdRangeExtensionsImpl<TRange, _TExtensionsImpl>
    {
        using Base = _MutFwdRangeExtensionsImpl<TRange, _TExtensionsImpl>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _MutBidiRangeExtensionsImpl<TRange, void>:
        public _MutBidiRangeExtensionsImpl<TRange, _BidiRangeExtensionsImpl<TRange>>
    {
        using Base = _MutBidiRangeExtensionsImpl<TRange, _BidiRangeExtensionsImpl<TRange>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TRangeExtensions = void>
    class MutBidiRangeExtensions: public MutFwdRangeExtensions<TRange, _TRangeExtensions>
    {
        using Base = MutFwdRangeExtensions<TRange, _TRangeExtensions>;

    protected:
        using _TImpl = typename Base::_TImpl;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename Base::TMutIter;
        using TMutIterEnd = typename Base::TMutIterEnd;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class MutBidiRangeExtensions<TRange, void>:
        public MutBidiRangeExtensions<TRange,
            BidiRangeExtensions<TRange, _MutBidiRangeExtensionsImpl<TRange>>>
    {
        using Base = MutBidiRangeExtensions<TRange,
            BidiRangeExtensions<TRange, _MutBidiRangeExtensionsImpl<TRange>>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}