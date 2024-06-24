export module atom.core:core.nums;

import std;
export import :core.int_wrapper;
export import :core.float_wrapper;

export namespace atom::nums
{
    template <typename num_type>
    constexpr auto get_min() -> num_type
    {
        return std::numeric_limits<num_type>::min();
    }

    template <typename num_type>
    constexpr auto get_max() -> num_type
    {
        return std::numeric_limits<num_type>::max();
    }

    template <typename num_type>
    constexpr auto get_min(num_type num0, num_type num1) -> num_type
    {
        return num0 < num1 ? num0 : num1;
    }

    template <typename num_type>
    constexpr auto get_max(num_type num0, num_type num1) -> num_type
    {
        return num0 > num1 ? num0 : num1;
    }

    template <typename num_type>
    constexpr auto get_abs(num_type num) -> num_type
    {
        return std::abs(num);
    }

    template <typename num_type>
    constexpr auto get_clamped(num_type num, num_type low, num_type high) -> num_type
    {
        return std::clamp(num, low, high);
    }
}