#pragma once
#include "Atom/Core.h"
#include "Atom/Engine/Window/Window.h"
#include "Atom/Memory.h"

namespace Atom::Engine
{
    class WindowProps
    {
    public:
        Str windowName;
        SWindowCoords windowSize;
    };

    class WindowManger
    {
    public:
        static auto CreateWindow(WindowProps props) -> Window*;

        static auto CloseWindow(Window* window) -> void;

    protected:
        static usize s_windowCount;
    };
}
