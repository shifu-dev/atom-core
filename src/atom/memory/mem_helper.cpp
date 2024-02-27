module;
#include "atom/preprocessors.h"

export module atom.core:mem_helper;
import :core;
import :ptr;
import :contracts_decl;
import :std;

export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// contains basic memory utility functions.
    /// --------------------------------------------------------------------------------------------
    class mem_helper
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// sets each byte of mem block `mem` with value `val`.
        ///
        /// @param mem: mem block to write to.
        /// @param mem_size: size of the mem block pointed by `mem`.
        /// @param val: value to write.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto fill(void* mem, usize mem_size, byte val) -> void
        {
            contracts::debug_expects(mem != nullptr);

            _fill(mem, mem_size, val);
        }

        /// ----------------------------------------------------------------------------------------
        /// same as {fill(...)}, but the call will not be optimized away.
        /// ----------------------------------------------------------------------------------------
        ATOM_PRAGMA_OPTIMIZE_OFF

        static constexpr auto fill_explicit(void* mem, usize mem_size, byte val) -> void
        {
            contracts::debug_expects(mem != nullptr);

            _fill(mem, mem_size, val);
        }

        ATOM_PRAGMA_OPTIMIZE_ON

        /// ----------------------------------------------------------------------------------------
        /// copies each byte from mem block `src` to mem block `dest` using fwd iteration.
        ///
        /// @param src: mem block to copy from.
        /// @param src_size: size of the mem block pointed by `src`.
        /// @param dest: mem block to copy to.
        /// @param dest_size: size of the mem block pointed by `dest`.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto fwd_copy_to(
            const void* src, usize src_size, void* dest, usize dest_size = usize::max()) -> void
        {
            contracts::debug_expects(src != nullptr);
            contracts::debug_expects(dest != nullptr);
            contracts::debug_expects(dest_size >= src_size);
            contracts::debug_expects(
                dest >= src and dest < (src + src_size), "dest mem block overlaps src mem block.");

            _fwd_copy(src, src_size, dest);
        }

        /// ----------------------------------------------------------------------------------------
        /// copies each byte from mem block `src` to mem block `dest` using bwd iteration.
        ///
        /// @param src: mem block to copy from.
        /// @param src_size: size of the mem block pointed by `src`.
        /// @param dest: mem block to copy to.
        /// @param dest_size: size of the mem block pointed by `dest`.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto bwd_copy_to(
            const void* src, usize src_size, void* dest, usize dest_size = usize::max()) -> void
        {
            contracts::debug_expects(src != nullptr);
            contracts::debug_expects(dest != nullptr);
            contracts::debug_expects(
                (dest + src_size) <= src and (dest + src_size) > (src + src_size),
                "dest mem block overlaps src mem block.");

            _bwd_copy(src, src_size, dest);
        }

        /// ----------------------------------------------------------------------------------------
        /// copies each byte from mem block `src` to mem block `dest` using appropriate
        /// iteration. this_type method is safe even if `src` and `dest` overlaps.
        ///
        /// @param src: mem block to copy from.
        /// @param src_size: size of the mem block pointed by `src`.
        /// @param dest: mem block to copy to.
        /// @param dest_size: size of the mem block pointed by `dest`.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto copy_to(
            const void* src, usize src_size, void* dest, usize dest_size = usize::max()) -> void
        {
            contracts::debug_expects(src != nullptr);
            contracts::debug_expects(dest != nullptr);
            contracts::debug_expects(dest_size >= src_size);

            if (dest > src)
            {
                _bwd_copy(src, src_size, dest);
            }
            else if (dest < src)
            {
                _fwd_copy(src, src_size, dest);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// copies each byte from mem block `mem` to mem block outset by `outset`.
        ///
        /// @param mem: mem block to copy from.
        /// @param mem_size: size of the mem block pointed by `mem`.
        /// @param outset: count of bytes outset from `mem`.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto copy_fwd(void* mem, usize mem_size, usize outset) -> void
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(outset > 0);

            _bwd_copy(mem, mem_size, (byte*)mem + outset);
        }

        /// ----------------------------------------------------------------------------------------
        /// copies each byte from mem block `mem` to mem block inset by `inset`.
        ///
        /// @param mem: mem block to copy from.
        /// @param mem_size: size of the mem block pointed by `mem`.
        /// @param inset: count of bytes inset from `mem`.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto copy_bwd(void* mem, usize mem_size, usize inset) -> void
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(inset > 0);

            _fwd_copy(mem, mem_size, (byte*)mem - inset);
        }

        /// ----------------------------------------------------------------------------------------
        /// copies each byte from mem block `mem` to mem block offset by `offset`.
        /// this_type method is safe even if src and dest overlaps.
        ///
        /// @param mem: mem block to copy from.
        /// @param mem_size: size of the mem block pointed by `mem`.
        /// @param offset: count of bytes offset from `mem`.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto copy_by(const void* mem, usize mem_size, isize offset) -> void
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(offset != 0);

            if (offset > 0)
            {
                _bwd_copy(mem, mem_size, (void*)mem + offset);
            }
            else
            {
                _fwd_copy(mem, mem_size, (void*)mem + offset);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// shifts each byte fwd in mem block `mem` by `steps` steps.
        ///
        /// @param mem: mem block to shift bytes of.
        /// @param mem_size: size of the mem block pointed by `mem`.
        /// @param steps: count of bytes to shift by.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto shift_fwd(void* mem, usize mem_size, usize steps) -> void
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps > 0);

            _shift_fwd(mem, mem_size, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// shifts each byte bwd in mem block `mem` by `steps` steps.
        ///
        /// @param mem: mem block to shift bytes of.
        /// @param mem_size: size of the mem block pointed by `mem`.
        /// @param steps: count of bytes to shift by.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto shift_bwd(void* mem, usize mem_size, usize steps) -> void
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps > 0);

            _shift_bwd(mem, mem_size, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// shifts each byte in mem block `mem` by `steps` steps.
        ///
        /// @param mem: mem block to shift bytes of.
        /// @param mem_size: size of the mem block pointed by `mem`.
        /// @param steps: count of bytes to shift by.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto shift_by(void* mem, usize mem_size, isize steps) -> void
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps != 0);

            if (steps > 0)
            {
                _shift_fwd(mem, mem_size, steps.to<usize>());
            }
            else
            {
                _shift_bwd(mem, mem_size, steps.abs().to<usize>());
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// rotates mem block `mem` fwd by `steps` steps.
        ///
        /// @param mem: mem block to rotate.
        /// @param mem_size: size of the mem block pointed by `mem`.
        /// @param steps: count of bytes to rotate by.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto rotate_fwd(void* mem, usize mem_size, usize steps) -> void
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps > 0);

            _rotate_fwd(mem, mem_size, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// rotates mem block `mem` bwd by `steps` steps.
        ///
        /// @param mem: mem block to rotate.
        /// @param mem_size: size of the mem block pointed by `mem`.
        /// @param steps: count of bytes to rotate by.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto rotate_bwd(void* mem, usize mem_size, usize steps) -> void
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps > 0);

            _rotate_bwd(mem, mem_size, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// rotates mem block `mem` by `steps` steps.
        ///
        /// @param mem: mem block to rotate.
        /// @param mem_size: size of the mem block pointed by `mem`.
        /// @param steps: count of bytes to rotate by.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto rotate_by(void* mem, usize mem_size, isize steps) -> void
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps != 0);

            if (steps > 0)
            {
                _shift_fwd(mem, mem_size, steps.to<usize>());
            }
            else
            {
                _shift_bwd(mem, mem_size, steps.abs().to<usize>());
            }
        }

    private:
        static constexpr auto _fill(void* mem, usize count, byte val) -> void
        {
            std::fill((byte*)mem, (byte*)mem + count, val);
        }

        static constexpr auto _fwd_copy(const void* src, usize count, void* dest) -> void
        {
            std::copy((byte*)src, (byte*)src + count, (byte*)dest);
        }

        static constexpr auto _bwd_copy(const void* src, usize count, void* dest) -> void
        {
            std::copy_backward((byte*)src, (byte*)src + count, (byte*)dest);
        }

        static constexpr auto _shift_fwd(void* mem, usize mem_size, usize steps) -> void
        {
            std::shift_right((byte*)mem, (byte*)mem + mem_size, steps.to_unwrapped());
        }

        static constexpr auto _shift_bwd(void* mem, usize mem_size, usize steps) -> void
        {
            std::shift_left((byte*)mem, (byte*)mem + mem_size, steps.to_unwrapped());
        }

        static constexpr auto _rotate_fwd(void* mem, usize mem_size, usize offset) -> void
        {
            std::rotate((byte*)mem, (byte*)mem + offset, (byte*)mem + mem_size);
        }

        static constexpr auto _rotate_bwd(void* mem, usize mem_size, usize offset) -> void
        {
            std::rotate((byte*)mem, (byte*)mem + offset, (byte*)mem + mem_size);
        }
    };
}
