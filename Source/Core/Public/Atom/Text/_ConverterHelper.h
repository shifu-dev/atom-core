export module Atom.Core.Text:Encodings._ConverterHelper;

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Requirements for {CharEncodingConverter} API.
    /// --------------------------------------------------------------------------------------------
    export template <typename TConverter, typename TInEncoding, typename TOutEncoding>
    concept RCharEncodingConverter = requires(TConverter converter,
        Internal::FwdRangeMock<BasicChar<TInEncoding>> in,
        Internal::OutputMock<BasicChar<TOutEncoding>> out)
    {
        requires RDefaultConstructible<TConverter>;

        { converter.Convert(in) }
            -> RSameAs<BasicString<TOutEncoding>>;

        { converter.Convert(in, out) }
            -> RSameAs<void>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {CharEncodingConverter<TInEncoding, TOutEncoding>} satisfies
    /// {RCharEncodingConverter<TInEncoding, TOutEncoding>}.
    /// --------------------------------------------------------------------------------------------
    export template <typename TInEncoding, typename TOutEncoding>
    concept RCharEncodingConvertible = RCharEncodingConverter<
        CharEncodingConverter<TInEncoding, TOutEncoding>,
        TInEncoding, TOutEncoding>;
}

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Converts data from {TInEncoding} character encoding to {TOutEncoding}.
    /// 
    /// @TPARAM TImpl Conversion impl.
    /// @TPARAM TInEncoding Character encoding to convert data from.
    /// @TPARAM TOutEncoding Character encoding to convert data to.
    /// --------------------------------------------------------------------------------------------
    template <typename TImpl, typename TInEncoding, typename TOutEncoding>
    struct _CharEncodingConverterHelper
    {
        using TInChar = typename TInEncoding::TChar;
        using TOutChar = typename TOutEncoding::TChar;
        using TOutString = Internal::String<TOutEncoding>;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInput, typename TOut>
        requires RFwdRange<TInput, TInChar> && ROutput<TOut, TOutChar>
        constexpr void ConvertTo(const TInput& in, TOut out)
        {
            auto end = in.end();
            for (auto it = in.begin(); it != end; it++)
            {
                TImpl::ConvertChar(it, out);
            }
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Specialization for same {CharEncoding}.
    /// --------------------------------------------------------------------------------------------
    template <typename TImpl, typename TEncoding>
    struct _CharEncodingConverterHelper<TImpl, TEncoding, TEncoding>
    {
        using TChar = BasicChar<TCharEncoding>;
        using TString = BasicString<TCharEncoding>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Writes input to output as is.
        /// ----------------------------------------------------------------------------------------
        template <typename TInput, typename TOut>
        requires RFwdRange<TInput, const TChar> && ROutput<TOut, TChar>
        constexpr void Convert(TInput&& in, TOut& out)
        {
            out += FORWARD(in);
        }
    };
}