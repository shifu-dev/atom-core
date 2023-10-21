#pragma once
#include "Atom/Exceptions.h"
#include "Atom/Hash/Private/T1Hash.h"
#include "Atom/Math.h"

namespace Atom::Private
{
    /// --------------------------------------------------------------------------------------------
    /// T1Hash Parser.
    /// --------------------------------------------------------------------------------------------
    template <typename T1Hash>
    class T1HashParser
    {
        static constexpr usize _Size = T1Hash::_Size;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Requires
        ///
        /// - `count == (_Size * 2) + 1`:  _Size * 2 (1 byte represents 2 hex values and 1 char
        /// represents one hex value) + 1 (null terminator).
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr auto Parse(const Char (&str)[count]) const -> T1Hash
            requires(count == (_Size * 2) + 1)
        {
            T1Hash hash;
            for (usize i = 0; i < _Size * 2; i += 2)
            {
                byte hex1 = Math::CharToHex(str[i + 0]);
                if (hex1 == (byte)-1)
                {
                    return T1Hash::Null;
                }

                // Left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;

                byte hex2 = Math::CharToHex(str[i + 1]);
                if (hex2 == (byte)-1)
                {
                    return T1Hash::Null;
                }

                hash.bytes[i / 2] = hex1 + hex2;
            }

            return hash;
        }

        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto Parse(const TRange& range) const -> T1Hash
            requires(RRangeOf<TRange, Char>)
        {
            T1Hash hash;
            usize i = 0;
            while (range.HasNext())
            {
                if (i > 20)
                {
                    return T1Hash::Null;
                }

                byte hex1 = Math::CharToHex(range.Get());
                if (hex1 == (byte)-1)
                {
                    return T1Hash::Null;
                }

                // Left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;

                if (!range.Next())
                {
                    return T1Hash::Null;
                }

                byte hex2 = Math::CharToHex(range.Get());
                if (hex2 == (byte)-1)
                {
                    return T1Hash::Null;
                }
                range.Next();

                hash.bytes[i++] = hex1 + hex2;
            }

            if (i < 20)
            {
                return T1Hash::Null;
            }

            return hash;
        }

        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto Parse(const TRange& range) const -> T1Hash
            requires(RJumpRangeOf<TRange, Char>)
        {
            if (range.Size() != _Size * 2)
            {
                return T1Hash::Null;
            }

            T1Hash hash;
            for (usize i = 0; i < 20; i++)
            {
                byte hex1 = Math::CharToHex(range.Get());
                if (hex1 == (byte)-1)
                {
                    return T1Hash::Null;
                }

                // Left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;
                range.Next();

                byte hex2 = Math::CharToHex(range.Get());
                if (hex2 == (byte)-1)
                {
                    return T1Hash::Null;
                }
                range.Next();

                hash.bytes[i] = hex1 + hex2;
            }

            return hash;
        }
    };
}