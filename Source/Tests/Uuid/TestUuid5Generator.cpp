#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid5Generator.h"
#include "Atom/Uuid/UuidHelper.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::Uuid5Generator")
{
    Uuid5Generator generator(UuidNamespaces::Oid);
    Uuid uuid1 = generator.generate("Hello");
    Uuid uuid2 = generator.generate("Hello");

    CHECK(uuid1 != Uuid::Null);
    CHECK(uuid1 == uuid2);

    CHECK(UuidHelper().getVersion(uuid1) == EUuidVersion::V5);
    CHECK(UuidHelper().getVersion(uuid2) == EUuidVersion::V5);
}