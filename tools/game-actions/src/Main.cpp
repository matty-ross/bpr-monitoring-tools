#include <Windows.h>

#include "GameActions.h"


static GameActions* s_GameActions = nullptr;


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
            s_GameActions = new GameActions;
        }
        break;

    case DLL_PROCESS_DETACH:
        {
            delete s_GameActions;
        }
        break;
    }

    return TRUE;
}