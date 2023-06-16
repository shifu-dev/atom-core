#pragma once
#include "StrView.h"
#include "StrConverter.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Converts {T} object to {StrView}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct StrViewConverter;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TConverter} can convert {T} object to {StrView}.
    /// --------------------------------------------------------------------------------------------
    template <typename TConverter, typename T>
    concept RStrViewConverter = requires(TConverter converter)
    {
        { converter.Convert(declval(T)) } -> RSameAs<StrView>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is convertible to {StrView}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RStrViewConvertible = RStrViewConverter<StrViewConverter<T>, T>;

    /// --------------------------------------------------------------------------------------------
    /// {StrViewConverter} specialization for {StrView}.
    /// --------------------------------------------------------------------------------------------
    template < >
    class StrViewConverter<StrView>
    {
        constexpr StrView Convert(StrView in) noexcept
        {
            return in;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {StirngViewConverter} specialization for {T} containing {const}, {volatile} or {lvalue} 
    /// and {rvalue} reference.
    /// 
    /// @TODO Needs refactoring.
    /// --------------------------------------------------------------------------------------------
	template <typename T>
	requires (!RSameAs<T, TTI::TRemoveCVRef<T>>) && RStrViewConvertible<TTI::TRemoveCVRef<T>>
	struct StrViewConverter<T>: StrViewConverter<TTI::TRemoveCVRef<T>> { };

    /// --------------------------------------------------------------------------------------------
    /// {StrConverter} specialization for types which are {StrViewConvertible}.
    /// --------------------------------------------------------------------------------------------
    template <RStrViewConvertible T>
    struct StrConverter<T>
    {
        constexpr Str Convert(const T& in) noexcept
        {
            return converter.Convert();
        }

        constexpr void Convert(const T& in, ROutput<Char> auto out) noexcept
        {
            StrView strView = converter.Convert(in);
            out.InsertBack(in);
        }

        StrViewConverter<T> converter;
    };
}