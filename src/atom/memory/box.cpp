export module atom.core:box;
import :_box_impl;
import :std;
import :core;
import :ptr;
import :mem_ptr;
import :memory.default_mem_allocator;

namespace atom
{
    template <typename in_impl_type>
    class box_functions
    {
    protected:
        using _impl_type = in_impl_type;

    public:
        using value_type = typename _impl_type::value_type;

    public:
        template <typename... arg_types>
        constexpr box_functions(arg_types&&... args)
            : _impl(forward<arg_types>(args)...)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr auto emplace(arg_types&&... args) -> type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            _impl.template emplace_val<type>(forward<arg_types>(args)...);
            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto set(type&& obj) -> type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            _impl._set_val(forward<type>(obj));
            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            _impl.destroy();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get() const -> const value_type&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.get_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut() -> value_type&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.get_val_mut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_get() const -> const value_type&
        {
            expects(has_val(), "value is null.");

            return _impl.get_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_get_mut() -> value_type&
        {
            expects(has_val(), "value is null.");

            return _impl.get_val_mut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_as() const -> const type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_mut_as() -> type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_get_as() const -> const type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_get_mut_as() -> type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> const ptr<value_type>
        {
            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_mem() -> ptr<value_type>
        {
            return _impl.mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mem() const -> const ptr<value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mut_mem() -> ptr<value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mem_as() const -> const ptr<type>
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            return _impl.template mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mut_mem_as() -> ptr<type>
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            return _impl.template mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mem_as() const -> const ptr<type>
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.template mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mut_mem_as() -> ptr<type>
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.template mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_type() const -> const std::type_info&
        {
            return _impl.obj_type();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_size() const -> usize
        {
            return _impl.obj_size();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto has_val() const -> bool
        {
            return _impl.has_val();
        }

    protected:
        _impl_type _impl;
    };
}

export namespace atom
{
    template <typename in_impl_type>
        requires(ris_void<typename in_impl_type::value_type>)
    class box_functions<in_impl_type>
    {
    protected:
        using _impl_type = in_impl_type;

    public:
        using value_type = void;

    public:
        template <typename... arg_types>
        constexpr box_functions(arg_types&&... args)
            : _impl(forward<arg_types>(args)...)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr auto emplace(arg_types&&... args) -> type&
            requires(not ris_void<type>)
        {
            _impl.template emplace_val<type>(forward<arg_types>(args)...);
            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto set(type&& obj) -> type&
        {
            _impl._set_val(forward<type>(obj));
            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            _impl.destroy_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_as() const -> const type&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_mut_as() -> type&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_get_as() const -> const type&
        {
            expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_get_mut_as() -> type&
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> mem_ptr<void>
        {
            return _impl.get_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_mem() -> mut_mem_ptr<void>
        {
            return _impl.get_mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mem() const -> mem_ptr<void>
        {
            expects(has_val(), "value is null.");

            return _impl.get_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mut_mem() -> mut_mem_ptr<void>
        {
            expects(has_val(), "value is null.");

            return _impl.get_mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mem_as() const -> const ptr<type>
            requires(not ris_void<type>)
        {
            return _impl.template mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mut_mem_as() -> ptr<type>
            requires(not ris_void<type>)
        {
            return _impl.template mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mem_as() const -> mem_ptr<void>
            requires(not ris_void<type>)
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mut_mem_as() -> mut_mem_ptr<void>
            requires(not ris_void<type>)
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_type() const -> const std::type_info&
        {
            return _impl.get_val_type();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_size() const -> usize
        {
            return _impl.obj_size();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto has_val() const -> bool
        {
            return _impl.has_val();
        }

    protected:
        _impl_type _impl;
    };

    template <typename value_type, usize buf_size = 50,
        typename allocator_type = default_mem_allocator>
    class box;

    template <typename value_type, usize buf_size = 50,
        typename allocator_type = default_mem_allocator>
    class copy_box;

    template <typename value_type, bool allow_non_move = true, usize buf_size = 50,
        typename allocator_type = default_mem_allocator>
    class move_box;

    template <typename value_type, bool allow_non_move = true, usize buf_size = 50,
        typename allocator_type = default_mem_allocator>
    class copy_move_box;

    template <typename value_type, usize buf_size, typename allocator_type>
    class box
        : public box_functions<_box_impl<value_type, false, false, false, buf_size, allocator_type>>
    {
        using this_type = box<value_type, buf_size, allocator_type>;
        using base_type =
            box_functions<_box_impl<value_type, false, false, false, buf_size, allocator_type>>;
        using _impl_type = typename base_type::_impl_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box()
            : base_type()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box(const this_type& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(const this_type& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box(this_type&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(this_type&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr box(const copy_box<type, that_buf_size, that_allocator_type>& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
            : base_type(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr this_type& operator=(
            const copy_box<type, that_buf_size, that_allocator_type>& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr box(const copy_move_box<type, true, that_buf_size, that_allocator_type>& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
            : base_type(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr this_type& operator=(
            const copy_move_box<type, true, that_buf_size, that_allocator_type>& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr box(move_box<type, true, that_buf_size, that_allocator_type>&& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
            : base_type(typename _impl_type::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr this_type& operator=(
            move_box<type, true, that_buf_size, that_allocator_type>&& that)
            requires(ris_void<value_type>) or rsame_or_derived_from<type, value_type>
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr box(copy_move_box<type, true, that_buf_size, that_allocator_type>&& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
            : base_type(typename _impl_type::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr this_type& operator=(
            copy_move_box<type, true, that_buf_size, that_allocator_type>&& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr box(type_holder<type> arg_type, arg_types&&... args)
            requires(ris_void<value_type> or rsame_or_derived_from<type, value_type>)
                    and rconstructible<type, arg_types...>
            : base_type(arg_type, forward<arg_types>(args)...)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr box(type&& obj)
            requires ris_void<value_type> or rsame_or_derived_from<pure_type<type>, value_type>
            : base_type(forward<type>(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr this_type& operator=(type&& obj)
            requires ris_void<value_type> or rsame_or_derived_from<pure_type<type>, value_type>
        {
            _impl.set_val(forward<type>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~box() {}

    private:
        using base_type::_impl;
    };

    template <typename value_type, usize buf_size, typename allocator_type>
    class copy_box
        : public box_functions<_box_impl<value_type, true, false, false, buf_size, allocator_type>>
    {
        using this_type = copy_box<value_type, buf_size, allocator_type>;
        using base_type =
            box_functions<_box_impl<value_type, true, false, false, buf_size, allocator_type>>;
        using _impl_type = typename base_type::_impl_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_box()
            : base_type()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_box(const this_type& that)
            : base_type(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(const this_type& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_box(this_type&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(this_type&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool allow_non_move, usize that_buf_size,
            typename that_allocator_type>
        constexpr copy_box(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
            : base_type(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool allow_non_move, usize that_buf_size,
            typename that_allocator_type>
        constexpr this_type& operator=(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr copy_box(type&& obj)
            : base_type(forward<type>(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr this_type& operator=(type&& obj)
            requires(rcopyable<type>)
        {
            _impl.set_val(forward<type>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~copy_box() {}

    private:
        using base_type::_impl;
    };

    template <typename value_type, bool allow_non_move, usize buf_size, typename allocator_type>
    class move_box
        : public box_functions<
              _box_impl<value_type, false, true, allow_non_move, buf_size, allocator_type>>
    {
        using this_type = move_box<value_type, allow_non_move, buf_size, allocator_type>;
        using base_type = box_functions<
            _box_impl<value_type, false, true, allow_non_move, buf_size, allocator_type>>;
        using _impl_type = typename base_type::_impl_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box()
            : base_type()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box(const this_type& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr move_box& operator=(const this_type& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr move_box(const copy_box<type, that_buf_size, that_allocator_type>& that)
            : base_type(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr move_box& operator=(
            const copy_box<type, that_buf_size, that_allocator_type>& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr move_box(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
            : base_type(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr move_box& operator=(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box(this_type&& that)
            : base_type(typename _impl_type::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr move_box& operator=(this_type&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr move_box(
            copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>&& that)
            : base_type(typename _impl_type::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr move_box& operator=(
            copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr move_box(type&& obj)
            : base_type(forward<type>(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr move_box& operator=(type&& obj)
        {
            _impl.set_val(forward<type>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~move_box() {}

    private:
        using base_type::_impl;
    };

    template <typename value_type, bool allow_non_move, usize buf_size, typename allocator_type>
    class copy_move_box
        : public box_functions<
              _box_impl<value_type, true, true, allow_non_move, buf_size, allocator_type>>
    {
        using this_type = copy_move_box<value_type, allow_non_move, buf_size, allocator_type>;
        using base_type = box_functions<
            _box_impl<value_type, true, true, allow_non_move, buf_size, allocator_type>>;
        using _impl_type = typename base_type::_impl_type;

    private:
        using base_type::_impl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box()
            : base_type()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box(const this_type& that)
            : base_type(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box& operator=(const this_type& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box(const copy_box<type, that_buf_size, that_allocator_type>& that)
            : base_type(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            const copy_box<type, that_buf_size, that_allocator_type>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
            : base_type(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box(this_type&& that)
            : base_type(typename _impl_type::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box& operator=(this_type&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box(
            move_box<type, that_allow_non_move, that_buf_size, that_allocator_type>&& that)
            : base_type(typename _impl_type::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            move_box<type, that_allow_non_move, that_buf_size, that_allocator_type>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box(
            copy_move_box<type, that_allow_non_move, that_buf_size, that_allocator_type>&& that)
            : base_type(typename _impl_type::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            copy_move_box<type, that_allow_non_move, that_buf_size, that_allocator_type>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr copy_move_box(type&& obj)
            : base_type(forward<type>(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr copy_move_box& operator=(type&& obj)
        {
            _impl.set_val(forward<type>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~copy_move_box() {}
    };
}
