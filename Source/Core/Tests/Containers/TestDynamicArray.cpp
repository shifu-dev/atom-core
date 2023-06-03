#include "catch2/catch_all.hpp"

#include "Atom/Containers/DynamicArray.h"

using namespace Atom;

TEST_CASE("Atom::Containers::DynamicArray")
{
    DynamicArray<int> arr;

    REQUIRE(arr.Count() == 0);
    REQUIRE(arr.Data() == nullptr);

//     REQUIRE(arr.Begin()         == arr.End());
//     REQUIRE(arr.ConstBegin()    == arr.ConstEnd());
//     REQUIRE(arr.RevBegin()      == arr.RevEnd());
//     REQUIRE(arr.ConstRevBegin() == arr.ConstRevEnd());
// 
//     arr.InsertAt(arr.Begin(), 0);
//     arr.InsertAt(arr.Begin(), { 1, 2 });
// 
//     arr.InsertFront(0);
//     arr.InsertFront({ 1, 2 });
// 
//     arr.InsertBack(0);
//     arr.InsertBack({ 1, 2 });
// 
//     arr.RemoveAt(arr.Begin());
//     arr.RemoveRange({ arr.Begin(), arr.End() });
}