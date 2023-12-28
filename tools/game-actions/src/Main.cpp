#include <Windows.h>

#include "GameActions.h"


GameActions* g_GameActions = nullptr;


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
            g_GameActions = new GameActions();
        }
        break;

    case DLL_PROCESS_DETACH:
        {
            delete g_GameActions;
        }
        break;
    }

    return TRUE;
}