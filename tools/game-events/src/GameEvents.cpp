#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <Windows.h>

#include "core/Logger.hpp"
#include "core/Patch.hpp"

#include "GameEvents.hpp"


extern "C" __declspec(dllexport) bool g_ExcludedGameEventIDs[500] = {};


GameEvents GameEvents::s_Instance;


GameEvents::GameEvents()
    :
    m_Logger(k_Name)
{
}

GameEvents& GameEvents::Get()
{
    return s_Instance;
}

void GameEvents::Load()
{
    try
    {
        Core::Logger::Initialize();

        FILE* newStdout = nullptr;
        freopen_s(&newStdout, "CONOUT$", "w", stdout);

        Core::Patch(0x00A254DD, 7, m_Logger).WriteJMP(HookPrintGameEvent);
    }
    catch (const std::exception& ex)
    {
        m_Logger.Error("%s", ex.what());
        MessageBoxA(NULL, ex.what(), k_Name, MB_ICONERROR);
    }
}

__declspec(naked) void GameEvents::HookPrintGameEvent()
{
    __asm
    {
        pushfd
        pushad

        push dword ptr [esi - 0xC]
        push dword ptr [esi - 0x10]
        push esi
        mov ecx, offset s_Instance
        call PrintGameEvent

        popad
        popfd

        // Original code.
        jmp dword ptr [ecx * 4 + 0x00A28E10]
    }
}

void GameEvents::PrintGameEvent(const std::byte* gameEvent, int32_t gameEventID, uint32_t gameEventSize) const
{
    if (!g_ExcludedGameEventIDs[gameEventID])
    {
        printf_s("%4d  [%4X] ", gameEventID, gameEventSize);
        for (uint32_t i = 0; i < gameEventSize; ++i)
        {
            printf_s(" %02X", gameEvent[i]);
        }
        putchar('\n');
    }
}
