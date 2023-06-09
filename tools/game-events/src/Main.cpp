#include <Windows.h>

#include "GameEvents.h"


static GameEvents* s_GameEvents = nullptr;


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
            s_GameEvents = new GameEvents();
        }
        break;

    case DLL_PROCESS_DETACH:
        {
            delete s_GameEvents;
        }
        break;
    }

    return TRUE;
}