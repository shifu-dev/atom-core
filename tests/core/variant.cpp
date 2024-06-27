import atom.core;

#include "helpers/tracked_type.h"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

using namespace atom;
using namespace atom::tests;
using namespace Catch;

using tracked_i32 = tracked_t_of<i32>;
using tracked_f32 = tracked_t_of<f32>;
using tracked_uchar = tracked_t_of<char>;

TEST_CASE("atom.core.variant")
{
    SECTION("unique types")
    {
        // compilation error.
        // variant only allows unique types.
        // using var = variant<tracked_i32, tracked_i32, tracked_f32>;
    }

    SECTION("type list")
    {
        using var = variant<tracked_i32, tracked_f32, tracked_uchar>;
        REQUIRE(typename var::value_types_list{}
                == type_list<tracked_i32, tracked_f32, tracked_uchar>{})
    }

    SECTION("trivial default constructor")
    {
        // # to do: fix this. check variant_impl default value for _index.
        //
        // STATIC_REQUIRE(is_trivially_default_constructible<
        //     variant<i32, char, f32>>);
    }

    SECTION("default constructor")
    {
        variant<tracked_i32, tracked_f32, tracked_uchar> v;

        REQUIRE(v.get_index() == 0);
        REQUIRE(v.is<tracked_i32>());
        REQUIRE(v.get<tracked_i32>().last_op == tracked_type::operation::default_constructor);
    }

    SECTION("trivial copy constructor")
    {
        STATIC_REQUIRE(type_info<variant<i32, char, f32>>::is_trivially_copy_constructible);
    }

    SECTION("copy constructor")
    {
        variant<tracked_i32, tracked_f32, tracked_uchar> v0 = tracked_f32{};
        variant<tracked_i32, tracked_f32, tracked_uchar> v1 = v0;

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor);
    }

    SECTION("copy constructor template")
    {
        variant<tracked_f32, tracked_uchar> v0 = tracked_f32{};
        variant<tracked_i32, tracked_f32, tracked_uchar> v1 = v0;

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor);
    }

    SECTION("trivial copy operator")
    {
        STATIC_REQUIRE(type_info<variant<i32, char, f32>>::is_trivially_copy_assignable);
    }

    SECTION("copy operator")
    {
        variant<tracked_i32, tracked_f32, tracked_uchar> v0 = tracked_f32{};

        // v1 holds tracked_i32, so it will construct tracked_f32 when assigned
        variant<tracked_i32, tracked_f32, tracked_uchar> v1;
        v1 = v0;

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor);

        // v1 holds tracked_f32, so it will assign tracked_f32 when now
        v1 = v0;

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_operator_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_operator);
    }

    SECTION("copy operator template")
    {
        variant<tracked_f32, tracked_uchar> v0 = tracked_f32{};

        // v1 holds tracked_i32, so it will construct tracked_f32 when assigned
        variant<tracked_i32, tracked_f32, tracked_uchar> v1;
        v1 = v0;

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor);

        // v1 holds tracked_f32, so it will assign tracked_f32 when now
        v1 = v0;

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_operator_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_operator);
    }

    SECTION("trivial move constructor")
    {
        STATIC_REQUIRE(type_info<variant<i32, char, f32>>::is_trivially_move_constructible);
    }

    SECTION("move constructor")
    {
        variant<tracked_i32, tracked_f32, tracked_uchar> v0 = tracked_f32{};
        variant<tracked_i32, tracked_f32, tracked_uchar> v1 = move(v0);

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_constructor);
    }

    SECTION("move constructor template")
    {
        variant<tracked_f32, tracked_uchar> v0 = tracked_f32{};
        variant<tracked_i32, tracked_f32, tracked_uchar> v1 = move(v0);

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_constructor);
    }

    SECTION("trivial move operator")
    {
        STATIC_REQUIRE(type_info<variant<i32, char, f32>>::is_trivially_move_assignable);
    }

    SECTION("move operator")
    {
        variant<tracked_i32, tracked_f32, tracked_uchar> v0 = tracked_f32{};

        // v1 holds tracked_i32, so it will construct tracked_f32 when assigned
        variant<tracked_i32, tracked_f32, tracked_uchar> v1;
        v1 = move(v0);

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_constructor);

        // v1 holds tracked_f32, so it will assign tracked_f32 when now
        v1 = move(v0);

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_operator_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_operator);
    }

    SECTION("move operator template")
    {
        variant<tracked_f32, tracked_uchar> v0 = tracked_f32{};

        // v1 holds tracked_i32, so it will construct tracked_f32 when assigned
        variant<tracked_i32, tracked_f32, tracked_uchar> v1;
        v1 = move(v0);

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_constructor);

        // v1 holds tracked_f32, so it will assign tracked_f32 when now
        v1 = move(v0);

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_operator_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_operator);
    }

    SECTION("param constructor")
    {
        variant<i32, f64, char> v = char('c');

        REQUIRE(v.get_index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.get<char>() == char('c'));
    }

    SECTION("param constructor: non deducible type.")
    {
        // compilation error.
        // 99 is an i32. use type explicitly.
        // variant<usize> v = 99;
        variant<usize> v = usize(99);

        REQUIRE(v.get_index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.get<usize>() == 99);
    }

    SECTION("param operator")
    {
        variant<i32, f64, char> v;
        v = char('c');

        REQUIRE(v.get_index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.get<char>() == char('c'));
    }

    SECTION("param operator: non deducible type.")
    {
        variant<usize> v;

        // compilation error.
        // 99 is an i32. use type explicitly.
        // v = 99;
        v = usize(99);

        REQUIRE(v.get_index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.get<usize>() == 99);
    }

    SECTION("destructor")
    {
        tracked_type::operation* last_op_ptr;

        {
            variant<tracked_i32, tracked_f32, tracked_uchar> v0 = tracked_f32{};
            last_op_ptr = &v0.get<tracked_f32>().last_op;
        }

        REQUIRE(*last_op_ptr == tracked_type::operation::destructor);
    }

    SECTION("value write")
    {
        variant<i32, f64, char> v;

        v.emplace<char>('h');

        REQUIRE(v.get_index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.get<char>() == char('h'));

        v.set(i32{ 99 });

        REQUIRE(v.get_index() == 0);
        REQUIRE(v.is<i32>());
        REQUIRE(v.get<i32>() == 99);
    }

    SECTION("value read")
    {
        variant<i32, f64, char> v = char('h');

        REQUIRE(v.is<char>());
        REQUIRE(v.get<char>() == char('h'));

        REQUIRE(v.is<2>());
        REQUIRE(v.get_at<2>() == char('h'));
    }
}
