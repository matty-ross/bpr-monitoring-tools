#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <Windows.h>

#include "core/Logger.hpp"
#include "core/Patch.hpp"

#include "GameEvents.hpp"


static constexpr char k_Name[] = "Game Events";


GameEvents GameEvents::s_Instance;


namespace Patches
{
    __declspec(naked) static void HookPrintGameEvent()
    {
        __asm
        {
            pushfd
            pushad

            push dword ptr [esi - 0xC]
            push dword ptr [esi - 0x10]
            push esi
            mov ecx, offset GameEvents::s_Instance
            call GameEvents::PrintGameEvent

            popad
            popfd

            // Original code.
            jmp dword ptr [ecx * 4 + 0x00A28E10]
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
        Core::Logger::Initialize();

        FILE* newStdout = nullptr;
        freopen_s(&newStdout, "CONOUT$", "w", stdout);

        Core::Patch(0x00A254DD, 7, m_Logger).WriteJMP(Patches::HookPrintGameEvent);
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

void GameEvents::PrintGameEvent(const std::byte* gameEvent, int32_t gameEventID, uint32_t gameEventSize) const
{
    printf_s("%4d  [%4X] ", gameEventID, gameEventSize);
    for (uint32_t i = 0; i < gameEventSize; ++i)
    {
        printf_s(" %02X", gameEvent[i]);
    }
    putchar('\n');
}
