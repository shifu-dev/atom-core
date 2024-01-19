#pragma once
#include "atom/range.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_elem_type>
    class basic_array_view
    {
        using this_type = basic_array_view;

    public:
        using elem_type = in_elem_type;
        using iter_type = array_iter<elem_type>;
        using iter_end_type = iter_type;
        using mut_iter_type = mut_array_iter<elem_type>;
        using mut_iter_end_type = mut_iter_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_array_view()
            : _data(nullptr)
            , _count(0)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_array_view(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr basic_array_view& operator=(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_array_view(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr basic_array_view& operator=(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # array constructor
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr basic_array_view(const elem_type (&arr)[count])
            : _data(arr)
            , _count(count)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # array operator
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr basic_array_view& operator=(const elem_type (&arr)[count])
        {
            _data = arr;
            _count = count;
        }

        /// ----------------------------------------------------------------------------------------
        /// # range constructor
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr basic_array_view(const range_type& range)
            requires(rarray_range_of<range_type, elem_type>)
            : _data(range.data())
            , _count(range.count())
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range operator
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr basic_array_view& operator=(const range_type& range)
            requires(rarray_range_of<range_type, elem_type>)
        {
            _data = range.data();
            _count = range.count();
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~basic_array_view() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() const -> mem_ptr<elem_type>
        {
            return _data;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_data() -> mut_mem_ptr<elem_type>
        {
            return _data;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto count() const -> usize
        {
            return _count;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter() const -> iter_type
        {
            return iter_type(_data);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter_end() const -> iter_end_type
        {
            return iter_end_type(_data + _count);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_iter() -> mut_iter_type
        {
            return mut_iter_type(_data);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_iter_end() -> mut_iter_end_type
        {
            return mut_iter_end_type(_data + _count);
        }

    private:
        mem_ptr<elem_type> _data;
        usize _count;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class array_view: public array_range_extensions<basic_array_view<elem_type>>
    {
        using base_type = array_range_extensions<basic_array_view<elem_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}