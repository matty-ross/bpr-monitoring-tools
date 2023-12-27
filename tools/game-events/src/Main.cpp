#include <Windows.h>

#include "GameEvents.h"


GameEvents* g_GameEvents = nullptr;


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
            g_GameEvents = new GameEvents();
        }
        break;

    case DLL_PROCESS_DETACH:
        {
            delete g_GameEvents;
        }
        break;
    }

    return TRUE;
}