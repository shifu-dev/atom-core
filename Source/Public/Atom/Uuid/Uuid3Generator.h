#pragma once
#include "Atom/Hash/Md5HashGenerator.h"
#include "Atom/Uuid/Private/UuidNameGenerator.h"
#include "Atom/Uuid/Uuid.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Generates version 3 Uuid using a name.
    /// --------------------------------------------------------------------------------------------
    using Uuid3Generator = Private::UuidNameGenerator<Md5Hash, Md5HashGenerator, EUuidVersion::V3>;
}