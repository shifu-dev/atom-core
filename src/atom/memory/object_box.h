#pragma once
#include "atom/core/requirements.h"
#include "atom/memory/default_mem_allocator.h"
#include "atom/tti.h"
#include "_box_impl.h"

namespace atom
{
    template <typename _timpl_>
    class box_functions
    {
    protected:
        using _timpl = _timpl_;

    public:
        using tval = typename _timpl::tval;

    public:
        constexpr box_functions(auto&&... args):
            _impl{ fwd(args)... }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t, typename... targs>
        constexpr auto emplace(targs&&... args) -> t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            _impl.template emplace_val<t>(fwd(args)...);
            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto set(t&& obj) -> t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            _impl._set_val(fwd(obj));
            return _impl.template get_mut_val_as<t>();
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
        constexpr auto get() const -> const tval&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.get_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut() -> tval&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.get_val_mut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_get() const -> const tval&
        {
            expects(has_val(), "value is null.");

            return _impl.get_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_get_mut() -> tval&
        {
            expects(has_val(), "value is null.");

            return _impl.get_val_mut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto get_as() const -> const t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto get_mut_as() -> t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_get_as() const -> const t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            expects(has_val(), "value is null.");

            return _impl.template get_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_get_mut_as() -> t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> const ptr<tval>
        {
            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_mem() -> ptr<tval>
        {
            return _impl.mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mem() const -> const ptr<tval>
        {
            expects(has_val(), "value is null.");

            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mut_mem() -> ptr<tval>
        {
            expects(has_val(), "value is null.");

            return _impl.mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto mem_as() const -> const ptr<t>
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            return _impl.template mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto mut_mem_as() -> ptr<t>
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            return _impl.template mut_mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_mem_as() const -> const ptr<t>
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            expects(has_val(), "value is null.");

            return _impl.template mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_mut_mem_as() -> ptr<t>
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            expects(has_val(), "value is null.");

            return _impl.template mut_mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_type() const -> const type_info&
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
        _timpl _impl;
    };

    template <typename _timpl_>
        requires(ris_void<typename _timpl_::tval>)
    class box_functions<_timpl_>
    {
    protected:
        using _timpl = _timpl_;

    public:
        using tval = void;

    public:
        constexpr box_functions(auto&&... args):
            _impl{ fwd(args)... }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t, typename... targs>
        constexpr auto emplace(targs&&... args) -> t&
            requires(not ris_void<t>)
        {
            _impl.template emplace_val<t>(fwd(args)...);
            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto set(t&& obj) -> t&
        {
            _impl._set_val(fwd(obj));
            return _impl.template get_mut_val_as<t>();
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
        template <typename t>
        constexpr auto get_as() const -> const t&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto get_mut_as() -> t&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_get_as() const -> const t&
        {
            expects(has_val(), "value is null.");

            return _impl.template get_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_get_mut_as() -> t&
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> const memptr
        {
            return _impl.get_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_mem() -> memptr
        {
            return _impl.get_mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mem() const -> const memptr
        {
            expects(has_val(), "value is null.");

            return _impl.get_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mut_mem() -> memptr
        {
            expects(has_val(), "value is null.");

            return _impl.get_mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto mem_as() const -> const ptr<t>
            requires(not ris_void<t>)
        {
            return _impl.template mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto mut_mem_as() -> ptr<t>
            requires(not ris_void<t>)
        {
            return _impl.template mut_mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_mem_as() const -> const memptr
            requires(not ris_void<t>)
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_mut_mem_as() -> memptr
            requires(not ris_void<t>)
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_type() const -> const type_info&
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
        _timpl _impl;
    };

    template <typename tval, usize buf_size = 50, typename talloc = default_mem_allocator>
    class box;

    template <typename tval, usize buf_size = 50, typename talloc = default_mem_allocator>
    class copy_box;

    template <typename tval, bool allow_non_move = true, usize buf_size = 50,
        typename talloc = default_mem_allocator>
    class move_box;

    template <typename tval, bool allow_non_move = true, usize buf_size = 50,
        typename talloc = default_mem_allocator>
    class copy_move_box;

    template <typename tval, usize buf_size, typename talloc>
    class box: public box_functions<_box_impl<tval, false, false, false, buf_size, talloc>>
    {
        using this = box<tval, buf_size, talloc>;
        using base = box_functions<_box_impl<tval, false, false, false, buf_size, talloc>>;
        using _timpl = typename base::_timpl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box():
            base{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box(const this& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this& operator=(const this& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box(this&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this& operator=(this&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr box(const copy_box<t, that_buf_size, tthat_alloc>& that)
            requires is_void<tval> or rsame_or_derived_from<t, tval>
            :
            base{ typename _timpl::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr this& operator=(const copy_box<t, that_buf_size, tthat_alloc>& that)
            requires is_void<tval> or rsame_or_derived_from<t, tval>
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr box(const copy_move_box<t, true, that_buf_size, tthat_alloc>& that)
            requires is_void<tval> or rsame_or_derived_from<t, tval>
            :
            base{ typename _timpl::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr this& operator=(const copy_move_box<t, true, that_buf_size, tthat_alloc>& that)
            requires is_void<tval> or rsame_or_derived_from<t, tval>
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr box(move_box<t, true, that_buf_size, tthat_alloc>&& that)
            requires is_void<tval> or rsame_or_derived_from<t, tval>
            :
            base{ typename _timpl::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr this& operator=(move_box<t, true, that_buf_size, tthat_alloc>&& that)
            requires (is_void<tval>) or rsame_or_derived_from<t, tval>
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr box(copy_move_box<t, true, that_buf_size, tthat_alloc>&& that)
            requires is_void<tval> or rsame_or_derived_from<t, tval>
            :
            base{ typename _timpl::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr this& operator=(copy_move_box<t, true, that_buf_size, tthat_alloc>&& that)
            requires is_void<tval> or rsame_or_derived_from<t, tval>
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, typename... targs>
        constexpr box(ctor_param<t> targ, targs&&... args)
            requires(is_void<tval> or rsame_or_derived_from<t, tval>) and rconstructible<t, targs...>
            :
            base{ targ, fwd(args)... }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr box(t&& obj)
            requires is_void<tval> or rsame_or_derived_from<tpure<t>, tval>
            :
            base{ fwd(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr this& operator=(t&& obj)
            requires is_void<tval> or rsame_or_derived_from<tpure<t>, tval>
        {
            _impl.set_val(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~box() {}

    private:
        using base::_impl;
    };

    template <typename tval, usize buf_size, typename talloc>
    class copy_box: public box_functions<_box_impl<tval, true, false, false, buf_size, talloc>>
    {
        using this = copy_box<tval, buf_size, talloc>;
        using base = box_functions<_box_impl<tval, true, false, false, buf_size, talloc>>;
        using _timpl = typename base::_timpl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_box():
            base{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_box(const this& that):
            base{ typename _timpl::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this& operator=(const this& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_box(this&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this& operator=(this&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool allow_non_move, usize that_buf_size, typename tthat_alloc>
        constexpr copy_box(const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that):
            base{ typename _timpl::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool allow_non_move, usize that_buf_size, typename tthat_alloc>
        constexpr this& operator=(const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr copy_box(t&& obj):
            base{ fwd(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr this& operator=(t&& obj)
            requires(rcopyable<t>)
        {
            _impl.set_val(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~copy_box() {}

    private:
        using base::_impl;
    };

    template <typename tval, bool allow_non_move, usize buf_size, typename talloc>
    class move_box: public box_functions<_box_impl<tval, false, true, allow_non_move, buf_size, talloc>>
    {
        using this = move_box<tval, allow_non_move, buf_size, talloc>;
        using base = box_functions<_box_impl<tval, false, true, allow_non_move, buf_size, talloc>>;
        using _timpl = typename base::_timpl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box():
            base{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box(const this& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr move_box& operator=(const this& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr move_box(const copy_box<t, that_buf_size, tthat_alloc>& that):
            base{ typename _timpl::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr move_box& operator=(const copy_box<t, that_buf_size, tthat_alloc>& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr move_box(const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that):
            base{ typename _timpl::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr move_box& operator=(
            const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box(this&& that):
            base{ typename _timpl::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr move_box& operator=(this&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr move_box(copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>&& that):
            base{ typename _timpl::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr move_box& operator=(copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr move_box(t&& obj):
            base{ fwd(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr move_box& operator=(t&& obj)
        {
            _impl.set_val(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~move_box() {}

    private:
        using base::_impl;
    };

    template <typename tval, bool allow_non_move, usize buf_size, typename talloc>
    class copy_move_box:
        public box_functions<_box_impl<tval, true, true, allow_non_move, buf_size, talloc>>
    {
        using this = copy_move_box<tval, allow_non_move, buf_size, talloc>;
        using base = box_functions<_box_impl<tval, true, true, allow_non_move, buf_size, talloc>>;
        using _timpl = typename base::_timpl;

    private:
        using base::_impl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box():
            base{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box(const this& that):
            base{ typename _timpl::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box& operator=(const this& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box(const copy_box<t, that_buf_size, tthat_alloc>& that):
            base{ typename _timpl::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box& operator=(const copy_box<t, that_buf_size, tthat_alloc>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box(const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that):
            base{ typename _timpl::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box(this&& that):
            base{ typename _timpl::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box& operator=(this&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool that_allow_non_move, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box(move_box<t, that_allow_non_move, that_buf_size, tthat_alloc>&& that):
            base{ typename _timpl::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool that_allow_non_move, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            move_box<t, that_allow_non_move, that_buf_size, tthat_alloc>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool that_allow_non_move, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box(copy_move_box<t, that_allow_non_move, that_buf_size, tthat_alloc>&& that):
            base{ typename _timpl::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool that_allow_non_move, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            copy_move_box<t, that_allow_non_move, that_buf_size, tthat_alloc>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr copy_move_box(t&& obj):
            base{ fwd(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr copy_move_box& operator=(t&& obj)
        {
            _impl.set_val(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~copy_move_box() {}
    };
}
