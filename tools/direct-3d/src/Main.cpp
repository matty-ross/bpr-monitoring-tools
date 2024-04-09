#include <Windows.h>

#include "Direct3D.hpp"


Direct3D* g_Direct3D = nullptr;


BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        {
            g_Direct3D = new Direct3D();
        }
        break;

    case DLL_PROCESS_DETACH:
        {
            delete g_Direct3D;
        }
        break;
    }

    return TRUE;
}
