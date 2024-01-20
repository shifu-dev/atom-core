export module atom.core:range.fwd_range_extensions;
import :range.range_extensions;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class _fwd_range_extensions_impl: public _range_extensions_impl<range_type>
    {
        using base_type = _range_extensions_impl<range_type>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _trange_extensions_impl = void>
    class fwd_range_extensions: public range_extensions<range_type, _trange_extensions_impl>
    {
        using base_type = range_extensions<range_type, _trange_extensions_impl>;

    protected:
        using _impl_type = typename base_type::_impl_type;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename range_type>
    class fwd_range_extensions<range_type, void>
        : public fwd_range_extensions<range_type, _fwd_range_extensions_impl<range_type>>
    {
        using base_type = fwd_range_extensions<range_type, _fwd_range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}