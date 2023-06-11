#pragma once
#include "Atom/Exceptions/Exceptions.h"
#include "Atom/Exceptions/Assertions.decl.h"

namespace Atom
{
    inline AssertException::AssertException(Str msg, StrView assertExpr) noexcept:
        assertExpr{ assertExpr }, Exception(MOVE(msg)) { }
}