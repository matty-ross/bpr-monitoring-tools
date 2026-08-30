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

        Core::Patch(0x00A254D1, 6, m_Logger).WriteJMP(Hook_PrintGameEvent);
    }
    catch (const std::exception& ex)
    {
        m_Logger.Error("%s", ex.what());
        MessageBoxA(NULL, ex.what(), k_Name, MB_ICONERROR);
    }
}

void GameEvents::PrintGameEvent(const std::byte* gameEvent, int32_t gameEventID, int32_t gameEventSize) const
{
    if (!g_ExcludedGameEventIDs[gameEventID])
    {
        printf_s("%4d  [%4X] ", gameEventID, gameEventSize);
        for (int32_t i = 0; i < gameEventSize; ++i)
        {
            printf_s(" %02X", gameEvent[i]);
        }
        putchar('\n');
    }
}

__declspec(naked) void GameEvents::Hook_PrintGameEvent()
{
    /*
        void __thiscall BrnGameState::GameStateModule::ProcessGameEvents(
            const BrnGameState::GameStateModuleIO::GameEventQueue* lpEventQueue,
            BrnGameState::GameStateModuleIO::BaseGameActionQueue<13312>* lpOutputActionQueue,
            const BrnGameState::GameStateModuleIO::PreWorldInputBuffer* lpInput,
            BrnGameState::GameStateModuleIO::OutputBuffer* lpOutput
        )
    */

    __asm
    {
        // esi: CgsModule::Event* lpEvent
        
        pushfd
        pushad

        lea eax, [esi - 0x10] // CgsModule::VariableEventQueue<5120, 16>::CBufferEntry* lpBufferEntry

        push dword ptr [eax + 0x4] // int32_t lpBufferEntry->miSize
        push dword ptr [eax + 0x0] // int32_t lpBufferEntry->miID
        push esi
        mov ecx, offset GameEvents::s_Instance
        call GameEvents::PrintGameEvent

        popad
        popfd

        // Original code.
        cmp ecx, 239

        // Jump back.
        push 0x00A254D7
        ret
    }
}
