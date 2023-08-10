#pragma once
#include "RangeReq.h"
#include "Atom/TTI.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    using StdIterCatForAtomIter = TTI::TEnableIf
    <
        RIter<TIter>,
        TTI::TNotConditional
        <
            RFwdIter<TIter>, std::input_iterator_tag,
            TTI::TNotConditional
            <
                RBidiIter<TIter>, std::forward_iterator_tag,
                TTI::TNotConditional
                <
                    RJumpIter<TIter>, std::bidirectional_iterator_tag,
                    TTI::TNotConditional
                    <
                        RArrIter<TIter>, std::random_access_iterator_tag,
                        std::contiguous_iterator_tag
                    >
                >
            >
        >
    >;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    class StdIterWrapForAtomIter
    {
        using Self = StdIterWrapForAtomIter<TIter>;

    public:
        using value_type = typename TIter::TElem;
        using size_type = usize;
        using difference_type = isize;
        using iterator_category = StdIterCatForAtomIter<TIter>;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        constexpr ctor StdIterWrapForAtomIter(const TIter& iter):
            iter{ iter } { }

        constexpr ctor StdIterWrapForAtomIter(TIter& iter):
            iter{ iter } { }

        constexpr ctor StdIterWrapForAtomIter(TIter&& iter):
            iter{ mov(iter) } { }

    public:
        template <class = void>
        constexpr fn operator*() const -> const value_type&
            requires(RIter<TIter>)
        {
            return *iter;
        }

        template <class = void>
        constexpr fn operator*() -> value_type&
            requires(RMutIter<TIter>)
        {
            return *iter;
        }

        template <class TIterEnd>
        constexpr fn operator== (const StdIterWrapForAtomIter<TIterEnd>& that) const -> bool
            requires(RIterEnd<TIter, TIterEnd>)
        {
            return iter == that.iter;
        }

        template <class TIterEnd>
        constexpr fn operator!= (const StdIterWrapForAtomIter<TIterEnd>& that) const -> bool
            requires(RIterEnd<TIter, TIterEnd>)
        {
            return iter != that.iter;
        }

        template <class = void>
        constexpr fn operator++() -> Self&
            requires(RIter<TIter>)
        {
            ++iter;
            return self;
        }

        template <class = void>
        constexpr fn operator++(i32) -> Self
            requires(RIter<TIter>)
        {
            return Self{ iter++ };
        }

        template <class = void>
        constexpr fn operator--() -> Self&
            requires(RBidiIter<TIter>)
        {
            --iter;
            return self;
        }

        template <class = void>
        constexpr fn operator--(i32) const -> Self
            requires(RBidiIter<TIter>)
        {
            return Self{ iter-- };
        }

        template <class = void>
        constexpr fn operator+(difference_type steps) -> Self
            requires(RJumpIter<TIter>)
        {
            return Self{ iter + steps };
        }

        template <class = void>
        constexpr fn operator-(difference_type steps) -> Self
            requires(RJumpIter<TIter>)
        {
            return Self{ iter - steps };
        }

        template <class = void>
        constexpr fn operator-(const Self& that) -> difference_type
            requires(RJumpIter<TIter>)
        {
            return iter - that.iter;
        }

    public:
        TIter iter;
    };
}