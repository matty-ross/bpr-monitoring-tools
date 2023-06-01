#include <Windows.h>

#include "NetworkTraffic.h"


static NetworkTraffic* s_NetworkTraffic = nullptr;


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
            DisableThreadLibraryCalls(hinstDLL);
            s_NetworkTraffic = new NetworkTraffic;
        }
        break;
    
    case DLL_PROCESS_DETACH:
        {
            delete s_NetworkTraffic;
        }
        break;
    }

    return TRUE;
}