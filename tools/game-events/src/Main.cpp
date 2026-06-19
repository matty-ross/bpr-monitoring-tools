#include <Windows.h>

#include "GameEvents.hpp"


BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        GameEvents::Get().OnProcessAttach();
        break;

    case DLL_PROCESS_DETACH:
        GameEvents::Get().OnProcessDetach();
        break;
    }

    return TRUE;
}
