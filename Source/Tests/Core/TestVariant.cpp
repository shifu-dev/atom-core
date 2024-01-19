#include "Atom/Core/Variant.h"
#include "Helpers/TrackedType.h"
#include "catch2/catch_all.hpp"
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace Atom;
using namespace Atom::Tests;
using namespace Catch;

using Tracked_i32 = TrackedTypeOf<i32>;
using Tracked_f32 = TrackedTypeOf<f32>;
using Tracked_Char = TrackedTypeOf<Char>;

TEST_CASE("Atom.Core.Variant")
{
    SECTION("Unique types")
    {
        // Compilation error.
        // Variant only allows unique types.
        // using Var = Variant<Tracked_i32, Tracked_i32, Tracked_f32>;
    }

    SECTION("Type Indexing")
    {
        using Var = Variant<Tracked_i32, Tracked_f32, Tracked_Char>;

        REQUIRE(Var::IndexOf<Tracked_i32>() == 0);
        REQUIRE(Var::IndexOf<Tracked_f32>() == 1);
        REQUIRE(Var::IndexOf<Tracked_Char>() == 2);

        REQUIRE(TTI::IsSame<Var::TAt<0>, Tracked_i32>);
        REQUIRE(TTI::IsSame<Var::TAt<1>, Tracked_f32>);
        REQUIRE(TTI::IsSame<Var::TAt<2>, Tracked_Char>);
    }

    SECTION("Count")
    {
        REQUIRE(Variant<i32, f64, f32, bool>::Count() == 4);
    }

    SECTION("Has")
    {
        using Var = Variant<Tracked_i32, Tracked_f32, Tracked_Char>;

        REQUIRE(Var::Has<Tracked_i32>());
        REQUIRE(Var::Has<Tracked_f32>());
        REQUIRE(Var::Has<Tracked_Char>());
        REQUIRE(not Var::Has<i32>());
        REQUIRE(not Var::Has<f32>());

        REQUIRE(Var::Has<0>());
        REQUIRE(Var::Has<1>());
        REQUIRE(Var::Has<2>());
        REQUIRE(not Var::Has<3>());
    }

    SECTION("Trivial Default Constructor")
    {
        // # To Do: Fix this. Check _VariantImpl default value for _index.
        //
        // STATIC_REQUIRE(RTriviallyDefaultConstructible<
        //     Variant<i32, Char, f32>>);
    }

    SECTION("Default Constructor")
    {
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v;

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<Tracked_i32>());
        REQUIRE(v.as<Tracked_i32>().lastOp == TrackedType::EOperation::DefaultConstructor);
    }

    SECTION("Trivial Copy Constructor")
    {
        STATIC_REQUIRE(RTriviallyCopyConstructible<Variant<i32, Char, f32>>);
    }

    SECTION("Copy Constructor")
    {
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v0 = Tracked_f32{};
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyConstructor);
    }

    SECTION("Template Copy Constructor")
    {
        Variant<Tracked_f32, Tracked_Char> v0 = Tracked_f32{};
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyConstructor);
    }

    SECTION("Trivial Copy Operator")
    {
        STATIC_REQUIRE(RTriviallyCopyAssignable<Variant<i32, Char, f32>>);
    }

    SECTION("Copy Operator")
    {
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v0 = Tracked_f32{};

        // v1 holds Tracked_i32, so it will construct Tracked_f32 when assigned
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v1;
        v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyConstructor);

        // v1 holds Tracked_f32, so it will assign Tracked_f32 when now
        v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyOperator);
    }

    SECTION("Template Copy Operator")
    {
        Variant<Tracked_f32, Tracked_Char> v0 = Tracked_f32{};

        // v1 holds Tracked_i32, so it will construct Tracked_f32 when assigned
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v1;
        v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyConstructor);

        // v1 holds Tracked_f32, so it will assign Tracked_f32 when now
        v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::CopyOperator);
    }

    SECTION("Trivial Move Constructor")
    {
        STATIC_REQUIRE(RTriviallyMoveConstructible<Variant<i32, Char, f32>>);
    }

    SECTION("Move Constructor")
    {
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v0 = Tracked_f32{};
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveConstructor);
    }

    SECTION("Template Move Constructor")
    {
        Variant<Tracked_f32, Tracked_Char> v0 = Tracked_f32{};
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveConstructor);
    }

    SECTION("Trivial Move Operator")
    {
        STATIC_REQUIRE(RTriviallyMoveAssignable<Variant<i32, Char, f32>>);
    }

    SECTION("Move Operator")
    {
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v0 = Tracked_f32{};

        // v1 holds Tracked_i32, so it will construct Tracked_f32 when assigned
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v1;
        v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveConstructor);

        // v1 holds Tracked_f32, so it will assign Tracked_f32 when now
        v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveOperator);
    }

    SECTION("Template Move Operator")
    {
        Variant<Tracked_f32, Tracked_Char> v0 = Tracked_f32{};

        // v1 holds Tracked_i32, so it will construct Tracked_f32 when assigned
        Variant<Tracked_i32, Tracked_f32, Tracked_Char> v1;
        v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveConstructor);

        // v1 holds Tracked_f32, so it will assign Tracked_f32 when now
        v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<Tracked_f32>());
        REQUIRE(v0.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<Tracked_f32>());
        REQUIRE(v1.as<Tracked_f32>().lastOp == TrackedType::EOperation::MoveOperator);
    }

    SECTION("Value Constructor")
    {
        Variant<i32, f64, Char> v = Char('c');

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<Char>());
        REQUIRE(v.as<Char>() == Char('c'));
    }

    SECTION("Value Constructor: Non deducible type.")
    {
        // Compilation error.
        // 99 is an i32. Use type explicitly.
        // Variant<usize> v = 99;
        Variant<usize> v = usize{ 99 };

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.as<usize>() == 99);
    }

    SECTION("Value Operator")
    {
        Variant<i32, f64, Char> v;
        v = Char('c');

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<Char>());
        REQUIRE(v.as<Char>() == Char('c'));
    }

    SECTION("Value Operator: Non deducible type.")
    {
        Variant<usize> v;

        // Compilation error.
        // 99 is an i32. Use type explicitly.
        // v = 99;
        v = usize{ 99 };

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.as<usize>() == 99);
    }

    SECTION("Destructor")
    {
        TrackedType::EOperation* lastOpPtr;

        {
            Variant<Tracked_i32, Tracked_f32, Tracked_Char> v0 = Tracked_f32{};
            lastOpPtr = &v0.as<Tracked_f32>().lastOp;
        }

        REQUIRE(*lastOpPtr == TrackedType::EOperation::Destructor);
    }

    SECTION("Value write")
    {
        Variant<i32, f64, Char> v;

        v.emplace<Char>('h');

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<Char>());
        REQUIRE(v.as<Char>() == Char('h'));

        v.set(i32{ 99 });

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<i32>());
        REQUIRE(v.as<i32>() == 99);
    }

    SECTION("Value Read")
    {
        Variant<i32, f64, Char> v = Char('h');

        REQUIRE(v.is<Char>());
        REQUIRE(v.as<Char>() == Char('h'));

        REQUIRE(v.is<2>());
        REQUIRE(v.at<2>() == Char('h'));
    }
}
