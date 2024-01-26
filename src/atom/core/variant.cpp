export module atom.core:variant;
import :_variant_impl;
import :core;
import :type_list;
import :contracts_decl;

export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// # to do
    /// - check requirements for assignments.
    /// - check if requirements using type_list functionality can be made concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    class variant
    {
        static_assert(type_list<types...>::are_unique, "every type in types... should be unique.");
        static_assert(type_list<types...>::count > 0, "at least one type needs to be specified.");

    private:
        using this_type = variant<types...>;
        using _impl_type = _variant_impl<types...>;

        template <typename... other_types>
        friend class variant;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type_list of this variant.
        /// ----------------------------------------------------------------------------------------
        using type_list = type_list<types...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// check if this variant supports type `type`.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        static consteval auto has() -> bool
        {
            return _impl_type::template has_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// check if index `index` can be used to access value.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        static consteval auto has() -> bool
        {
            return _impl_type::template has_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// get type at index.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
            requires(has<index>())
        using type_at = typename _impl_type::template type_at_index<index>;

        /// ----------------------------------------------------------------------------------------
        /// get first type.
        /// ----------------------------------------------------------------------------------------
        using first_type = typename _impl_type::first_type;

        /// ----------------------------------------------------------------------------------------
        /// get first type.
        /// ----------------------------------------------------------------------------------------
        using last_type = typename _impl_type::last_type;

        /// ----------------------------------------------------------------------------------------
        /// index of type. this index than can be used to access value of type at that index.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        static consteval auto index_of() -> usize
            requires(has<type>())
        {
            return _impl_type::template get_index_for_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// count of types this variant supports.
        /// ----------------------------------------------------------------------------------------
        static consteval auto count() -> usize
        {
            return _impl_type::get_type_count();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant()
            requires(rdefault_constructible<first_type>) or (ris_void<first_type>)
        {
            _impl.template construct_value_by_index<0>();
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(const variant& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(const variant& that)
            requires(rcopy_constructible_all<types...>)
                    and (not rtrivially_copy_constructible_all<types...>)
        {
            _impl.construct_value_from_variant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        constexpr variant(const variant<other_types...>& that)
            requires(rcopy_constructible_all<other_types...>)
                    and (type_list::template has<other_types...>)
        {
            _impl.construct_value_from_variant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr variant& operator=(const variant& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr variant& operator=(const variant& that)
            requires(rcopyable_all<types...>) and (not rtrivially_copy_assignable_all<types...>)
        {
            _impl.set_value_from_variant(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        constexpr variant& operator=(const variant<other_types...>& that)
            requires(rcopyable_all<other_types...>) and (type_list::template has<other_types...>)
        {
            _impl.set_value_from_variant(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(variant&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(variant&& that)
            requires(rmove_constructible_all<types...>)
                    and (not rtrivially_move_constructible_all<types...>)
        {
            _impl.construct_value_from_variant(move(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        constexpr variant(variant<other_types...>&& that)
            requires(rmove_constructible_all<other_types...>)
                    and (type_list::template has<other_types...>)
        {
            _impl.construct_value_from_variant(move(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr variant& operator=(variant&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr variant& operator=(variant&& that)
            requires(rmoveable_all<types...>) and (not rtrivially_move_assignable_all<types...>)
        {
            _impl.set_value_from_variant(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        constexpr variant& operator=(variant<other_types...>&& that)
            requires(rmoveable_all<types...>) and (type_list::template has<other_types...>)
        {
            _impl.set_value_from_variant(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value copy constructor
        ///
        /// copy constructs variant with value of type type.
        ///
        /// # parameters
        /// - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr variant(const type& value)
            requires(has<type>())
        {
            _impl.template construct_value_by_type<type>(value);
        }

        /// ----------------------------------------------------------------------------------------
        /// # value move constructor
        ///
        /// move constructs variant with value of type type.
        ///
        /// # parameters
        /// - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr variant(type&& value)
            requires(has<type>())
        {
            _impl.template construct_value_by_type<type>(move(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value copy operator
        ///
        /// destroys previous value assigns new value.
        ///
        /// # parameters
        /// - `value`: value to assign.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr variant& operator=(const type& value)
            requires(has<type>())
        {
            _impl.set_value(value);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value move operator
        ///
        /// destroys previous value assigns new value.
        ///
        /// # parameters
        /// - `value`: value to assign.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr variant& operator=(type&& value)
            requires(has<type>())
        {
            _impl.set_value(move(value));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~variant() = default;

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        ///
        /// destructs value.
        /// ----------------------------------------------------------------------------------------
        constexpr ~variant()
            requires(not rtrivially_destructible_all<types...>)
        {
            _impl.destroy_value();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// constructs the type `type` and sets the value.
        ///
        /// # see also
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr auto emplace(arg_types&&... args)
            requires(has<type>()) and (rconstructible<type, arg_types...>)
        {
            _impl.template emplace_value_by_type<type>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs the type for index `index` and sets the value.
        ///
        /// # see also
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize index, typename... arg_types>
        constexpr auto emplace(arg_types&&... args)
            requires(has<index>()) and (rconstructible<type_at<index>, arg_types...>)
        {
            _impl.template emplace_value_by_index<index>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the value to `value`.
        ///
        /// # parameters
        /// - `value`: value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto set(const type&& value)
            requires(has<type>()) and (rcopy_constructible<type>)
        {
            _impl.set_value(value);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the value to `value`.
        ///
        /// # parameters
        /// - `value`: value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto set(type&& value)
            requires(has<type>()) and (rmove_constructible<type>)
        {
            _impl.set_value(move(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value as type `type`.
        ///
        /// # template parameters
        /// - `type`: type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto as() const -> const type&
            requires(has<type>()) and (not ris_void<type>)
        {
            contracts::expects(is<type>(), "access to invalid type.");

            return _impl.template get_value_by_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value as type `type`.
        ///
        /// # template parameters
        /// - `type`: type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto as() -> type&
            requires(has<type>()) and (not ris_void<type>)
        {
            contracts::debug_expects(is<type>(), "access to invalid type.");

            return _impl.template get_value_by_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto as_check() const -> const type&
            requires(has<type>()) and (not ris_void<type>)
        {
            contracts::expects(is<type>(), "access to invalid type.");

            return _impl.template get_value_by_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto as_check() -> type&
            requires(has<type>()) and (not ris_void<type>)
        {
            contracts::expects(is<type>(), "access to invalid type.");

            return _impl.template get_value_by_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value at index `index`.
        ///
        /// # template parameters
        /// - `index`: index of type to access variants value as.
        ///
        /// # see also
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto at() const -> const type_at<index>&
            requires(has<index>()) and (not ris_void<type_at<index>>)
        {
            contracts::expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value at index `index`.
        ///
        /// # template parameters
        /// - `index`: index of type to access variants value as.
        ///
        /// # see also
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto at() -> type_at<index>&
            requires(has<index>()) and (not ris_void<type_at<index>>)
        {
            contracts::debug_expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto at_check() const -> const type_at<index>&
            requires(has<index>()) and (not ris_void<type_at<index>>)
        {
            contracts::expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto at_check() -> type_at<index>&
            requires(has<index>()) and (not ris_void<type_at<index>>)
        {
            contracts::expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if current value is of type `type`.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto is() const -> bool
            requires(has<type>())
        {
            return _impl.template is_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if current value is of type accessed by index `index`.
        ///
        /// # see also
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto is() const -> bool
            requires(has<index>())
        {
            return _impl.template is_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// get the index to current type.
        /// ----------------------------------------------------------------------------------------
        constexpr auto index() const -> usize
        {
            return _impl.get_type_index();
        }

    private:
        _impl_type _impl;
    };
}
