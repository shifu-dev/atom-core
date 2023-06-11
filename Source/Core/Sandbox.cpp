#include "Atom/Str/StrView.h"

using namespace Atom;

struct Console
{
    void Out(StrView str)
    {
        std::wcout << std::wstring_view((const wchar_t*)str.Data(), str.Count()) << std::endl;
    }
};

static Console Console;

int main()
{
    return 0;
}