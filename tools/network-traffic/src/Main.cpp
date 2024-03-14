#include <Windows.h>

#include "NetworkTraffic.hpp"


NetworkTraffic* g_NetworkTraffic = nullptr;


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
            g_NetworkTraffic = new NetworkTraffic();
        }
        break;
    
    case DLL_PROCESS_DETACH:
        {
            delete g_NetworkTraffic;
        }
        break;
    }

    return TRUE;
}
