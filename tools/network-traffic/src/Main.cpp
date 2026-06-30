#include <Windows.h>

#include "NetworkTraffic.hpp"


BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        NetworkTraffic::Get().Load();
        break;

    case DLL_PROCESS_DETACH:
        NetworkTraffic::Get().Unload();
        break;
    }

    return TRUE;
}
