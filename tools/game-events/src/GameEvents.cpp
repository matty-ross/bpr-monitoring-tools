#include <cstdint>
#include <cstdio>
#include <exception>
#include <Windows.h>

#include "core/Pointer.hpp"
#include "core/Patch.hpp"

#include "GameEvents.hpp"


static constexpr char k_Name[] = "Game Events";


GameEvents GameEvents::s_Instance;


namespace Patches
{
    __declspec(naked) static void HookProcessGameEvents()
    {
        __asm
        {
            pushfd
            pushad

            push dword ptr [esi - 0xC]
            push dword ptr [esi - 0x10]
            push esi
            mov ecx, offset GameEvents::s_Instance
            call GameEvents::OnGameEvent

            popad
            popfd

            push 0x00A254E4
            ret
        }
    }
}


GameEvents::GameEvents()
    :
    m_Logger(k_Name)
{
}

GameEvents& GameEvents::Get()
{
    return s_Instance;
}

void GameEvents::OnProcessAttach()
{
    try
    {
        Core::Patch(0x00A254DD, 7, m_Logger).WriteJMP(Patches::HookProcessGameEvents);
    }
    catch (const std::exception& ex)
    {
        m_Logger.Error("%s", ex.what());
        MessageBoxA(NULL, ex.what(), k_Name, MB_ICONERROR);
    }
}

void GameEvents::OnProcessDetach()
{
}

void GameEvents::OnGameEvent(void* gameEvent, int32_t gameEventID, uint32_t gameEventSize) const
{
    printf_s("%4d  [%4X] ", gameEventID, gameEventSize);
    for (uint32_t i = 0; i < gameEventSize; ++i)
    {
        printf_s(" %02X", Core::Pointer(gameEvent).at(i).as<uint8_t>());
    }
    putchar('\n');
}
