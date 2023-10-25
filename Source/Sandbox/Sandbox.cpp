// #include "Atom/CoreAll.h"
#include "Atom/Core/Primitives.h"
#include <iostream>

using namespace Atom;

auto PrintCmdArgs(i32 argc, char** argv)
{
    for (i32 i = 1; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }
}

auto main(int argc, char** argv) -> int
{
    PrintCmdArgs(argc, argv);

    return 0;
}
