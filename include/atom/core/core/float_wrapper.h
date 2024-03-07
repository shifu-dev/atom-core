#pragma once
#include "atom/core/_std.h"
#include "atom/core/core/char_wrapper.h"
#include "atom/core/core/num_wrapper.h"
#include "atom/core/preprocessors.h"

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename in_final_type, typename in_unwrapped_type>
    class _float_wrapper_impl
    {
    public:
        using final_type = in_final_type;
        using unwrapped_type = in_unwrapped_type;

    public:
        static consteval auto min() -> unwrapped_type
        {
            return unwrapped_type(std::numeric_limits<unwrapped_type>::min());
        }

        static consteval auto max() -> unwrapped_type
        {
            return unwrapped_type(std::numeric_limits<unwrapped_type>::max());
        }

        static consteval auto bits() -> unwrapped_type
        {
            return unwrapped_type(sizeof(unwrapped_type) * 8);
        }

        static consteval auto nan() -> unwrapped_type
        {
            return unwrapped_type();
        }

        static constexpr auto floor(unwrapped_type val) -> unwrapped_type
        {
            return std::floor(val);
        }

        static constexpr auto ceil(unwrapped_type val) -> unwrapped_type
        {
            return std::ceil(val);
        }

        static constexpr auto round(unwrapped_type val) -> unwrapped_type
        {
            return std::round(val);
        }

        static constexpr auto is_add_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            return true;
        }

        static constexpr auto is_sub_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            return true;
        }

        static constexpr auto is_mul_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            return true;
        }

        static constexpr auto is_div_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            return true;
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_from(num_type num) -> bool
        {
            return is_conversion_safe_from_unwrapped<typename num_type::unwrapped_type>(
                num.to_unwrapped());
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_from_unwrapped(num_type num) -> bool
        {
            return true;
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_to(num_type num) -> bool
        {
            return true;
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_to_unwrapped(num_type num) -> bool
        {
            return true;
        }
    };
}

/// ------------------------------------------------------------------------------------------------
/// final types
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    using _f16 = float;
    using _f32 = float;
    using _f64 = double;
    using _f128 = long double;

    ATOM_ALIAS(f16, num_wrapper<_float_wrapper_impl<f16, _f16>>);
    ATOM_ALIAS(f32, num_wrapper<_float_wrapper_impl<f32, _f32>>);
    ATOM_ALIAS(f64, num_wrapper<_float_wrapper_impl<f64, _f64>>);
    ATOM_ALIAS(f128, num_wrapper<_float_wrapper_impl<f128, _f128>>);
}