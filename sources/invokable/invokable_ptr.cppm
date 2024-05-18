export module atom.core:invokable_ptr;

namespace atom
{
    template <typename... tsign>
    class _invokable_ptr_impl;

    template <typename result_t, typename... arg_ts>
    class _invokable_ptr_impl<result_t(arg_ts...)>
    {
    public:
        using type = result_t (*)(arg_ts...);
    };

    export template <typename... tsign>
    using invokable_ptr = typename _invokable_ptr_impl<tsign...>::type;
}