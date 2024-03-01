export module atom.core:variant;
import :core;
import :type_list;
import :contracts_decl;
import :ptr;
import :obj_helper;
import :static_storage;
import :tti;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename... types>
    class _variant_storage
    {
    public:
        constexpr auto get_data() -> void*
        {
            return &_storage.storage;
        }

        constexpr auto get_data() const -> const void*
        {
            return &_storage.storage;
        }

    private:
        static_storage_for<types...> _storage;
    };

    /// --------------------------------------------------------------------------------------------
    /// implementatiion of [`variant`].
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    class _variant_impl
    {
    private:
        template <typename... other_types>
        friend class _variant_impl;

    private:
        using _type_list = type_list<types...>;
        using _storage_type = _variant_storage<types...>;

    public:
        static constexpr auto get_type_count() -> usize
        {
            return _type_list::count;
        }

        template <typename type>
        static consteval auto has_type() -> bool
        {
            return _type_list::template has<type>;
        }

        template <usize i>
        static consteval auto has_index() -> bool
        {
            return i < _type_list::count;
        }

        template <typename type>
        static consteval auto get_index_for_type() -> usize
        {
            return _type_list::template index_of<type>;
        }

        static consteval auto get_null_type_index() -> usize
        {
            return usize::max();
        }

        template <usize i>
        using type_at_index = typename _type_list::template at<i>;

        using first_type = type_at_index<0>;

        using last_type = type_at_index<get_type_count() - 1>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// copy or mov constructs value hold by variant `that`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        constexpr auto construct_value_from_variant(const _variant_impl<other_types...>& that)
        {
            _construct_value_from_variant_impl<false, 0, other_types...>(
                that, that.get_type_index());
        }

        template <typename... other_types>
        constexpr auto construct_value_from_variant(_variant_impl<other_types...>&& that)
        {
            _construct_value_from_variant_impl<true, 0, other_types...>(
                that, that.get_type_index());
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs or assigns value from `that` variant.
        /// assigns if `that` variant holds the same type else constructs.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        constexpr auto set_value_from_variant(const _variant_impl<other_types...>& that)
        {
            _set_value_from_variant_impl<false, 0, other_types...>(that, that.get_type_index());
        }

        template <typename... other_types>
        constexpr auto set_value_from_variant(_variant_impl<other_types...>&& that)
        {
            _set_value_from_variant_impl<true, 0, other_types...>(that, that.get_type_index());
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type `type` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr auto construct_value_by_type(arg_types&&... args)
        {
            _construct_value_as<type>(forward<arg_types>(args)...);
            _index = get_index_for_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type at index `i` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... arg_types>
        constexpr auto construct_value_by_index(arg_types&&... args)
        {
            construct_value_by_type<type_at_index<i>>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type `type` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr auto emplace_value_by_type(arg_types&&... args)
        {
            destroy_value();

            _construct_value_as<type>(forward<arg_types>(args)...);
            _index = get_index_for_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type at index `i` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... arg_types>
        constexpr auto emplace_value_by_index(arg_types&&... args)
        {
            emplace_value_by_type<type_at_index<i>>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs or assigns value of type deduced by `value` with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto set_value(value_type&& value)
        {
            using type = tti::remove_quailfiers_ref_type<value_type>;
            usize index_to_set = get_index_for_type<type>();

            // the new type to set is same as current.
            if (index_to_set == _index)
            {
                _assign_value_as<type>(forward<value_type>(value));
            }
            else
            {
                destroy_value();
                construct_value_by_type<type>(forward<value_type>(value));
            }
        }

        template <typename type>
        constexpr auto get_value_by_type() const -> const type&
        {
            contracts::debug_expects(get_index_for_type<type>() == get_type_index(),
                "current type is not same as requested type.");

            return _get_value_as<type>();
        }

        template <typename type>
        constexpr auto get_value_by_type() -> type&
        {
            contracts::debug_expects(get_index_for_type<type>() == get_type_index(),
                "current type is not same as requested type.");

            return _get_value_as<type>();
        }

        template <usize i>
        constexpr auto get_value_by_index() const -> const type_at_index<i>&
        {
            return get_value_by_type<type_at_index<i>>();
        }

        template <usize i>
        constexpr auto get_value_by_index() -> type_at_index<i>&
        {
            return get_value_by_type<type_at_index<i>>();
        }

        template <typename type>
        constexpr auto is_type() const -> bool
        {
            return get_index_for_type<type>() == get_type_index();
        }

        template <usize i>
        constexpr auto is_index() const -> bool
        {
            return i == get_type_index();
        }

        constexpr auto get_type_index() const -> usize
        {
            return _index;
        }

        constexpr auto destroy_value()
        {
            _destroy_value_impl<0, types...>(get_type_index());
        }

    private:
        template <bool mov, usize index, typename other_type, typename... other_types>
        constexpr auto _construct_value_from_variant_impl(auto& that, usize that_index)
        {
            using that_types = type_list<other_types...>;

            if (index != that_index)
            {
                if constexpr (that_types::count == 0)
                {
                    panic("there is no type for current index.");
                }
                else
                {
                    _construct_value_from_variant_impl<mov, index + 1, other_types...>(
                        that, that_index);
                    return;
                }
            }

            if constexpr (mov)
            {
                _construct_value_as<other_type>(move(that.template _get_value_as<other_type>()));
            }
            else
            {
                _construct_value_as<other_type>(that.template _get_value_as<other_type>());
            }

            _index = get_index_for_type<other_type>();
        }

        template <bool mov, usize index, typename other_type, typename... other_types>
        constexpr auto _set_value_from_variant_impl(auto&& that, usize that_index)
        {
            using that_types = type_list<other_types...>;

            if (index != that_index)
            {
                if constexpr (that_types::count == 0)
                {
                    panic("there is no type for current index.");
                }
                else
                {
                    _set_value_from_variant_impl<mov, index + 1, other_types...>(that, that_index);
                    return;
                }
            }

            // index for this variant of type same as that `variant` current type.
            usize index_for_this = get_index_for_type<other_type>();

            // we already have this type, so we don'type construct it but assign it.
            if (_index == index_for_this)
            {
                if constexpr (mov)
                {
                    _assign_value_as<other_type>(move(that.template _get_value_as<other_type>()));
                }
                else
                {
                    _assign_value_as<other_type>(that.template _get_value_as<other_type>());
                }
            }
            else
            {
                destroy_value();

                if constexpr (mov)
                {
                    _construct_value_as<other_type>(
                        move(that.template _get_value_as<other_type>()));
                }
                else
                {
                    _construct_value_as<other_type>(that.template _get_value_as<other_type>());
                }

                _index = index_for_this;
            }
        }

        template <usize index, typename type, typename... other_types>
        constexpr auto _destroy_value_impl(usize i)
        {
            using _type_list = type_list<other_types...>;

            if (i != index)
            {
                if constexpr (_type_list::count == 0)
                {
                    panic("there is no type for current index.");
                }
                else
                {
                    // recursion to find type at index i.
                    _destroy_value_impl<index + 1, other_types...>(i);
                    return;
                }
            }

            _destruct_value_as<type>();
        }

        template <typename type, typename... arg_types>
        constexpr auto _construct_value_as(arg_types&&... args)
        {
            obj_helper().construct(_get_data_as<type>(), forward<arg_types>(args)...);
        }

        template <typename type, typename other_type>
        constexpr auto _assign_value_as(other_type&& val)
        {
            obj_helper().assign(_get_data_as<type>(), forward<other_type>(val));
        }

        template <typename type>
        constexpr auto _destruct_value_as()
        {
            if constexpr (not rsame_as<type, void>)
                obj_helper().destruct(_get_data_as<type>());
        }

        template <typename type>
        constexpr auto _get_value_as() -> type&
        {
            return *_get_data_as<type>();
        }

        template <typename type>
        constexpr auto _get_value_as() const -> const type&
        {
            return *_get_data_as<type>();
        }

        template <typename type>
        constexpr auto _get_data_as() -> type*
        {
            return (type*)_storage.get_data();
        }

        template <typename type>
        constexpr auto _get_data_as() const -> const type*
        {
            return (const type*)_storage.get_data();
        }

    private:
        _storage_type _storage;
        usize _index = 0;
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
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
        static consteval auto get_count() -> usize
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
        ///
        ///  - `value`: value to construct with.
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
        ///
        ///  - `value`: value to construct with.
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
        ///
        ///  - `value`: value to assign.
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
        ///
        ///  - `value`: value to assign.
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
        ///
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
        ///
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
        ///
        ///  - `value`: value to set.
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
        ///
        ///  - `value`: value to set.
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
        ///
        ///  - `type`: type to access variant's value as.
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
        ///
        ///  - `type`: type to access variant's value as.
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
        ///
        ///  - `index`: index of type to access variants value as.
        ///
        /// # see also
        ///
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at() const -> const type_at<index>&
            requires(has<index>()) and (not ris_void<type_at<index>>)
        {
            contracts::expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value at index `index`.
        ///
        /// # template parameters
        ///
        ///  - `index`: index of type to access variants value as.
        ///
        /// # see also
        ///
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at() -> type_at<index>&
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
        ///
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
