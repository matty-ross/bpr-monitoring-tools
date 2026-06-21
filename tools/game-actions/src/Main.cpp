#include <Windows.h>

#include "GameActions.hpp"


BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        GameActions::Get().OnProcessAttach();
        break;

    case DLL_PROCESS_DETACH:
        GameActions::Get().OnProcessDetach();
        break;
    }

    return TRUE;
}
