#include "Atom/Engine/Application.h"

using namespace Atom;

auto main(i32 argc, char** args) -> i32
{
    Engine::Application* app = Engine::CreateApplication();
    app->Run();

    delete app;
}
