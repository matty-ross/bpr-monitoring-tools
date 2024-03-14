#include "GameEvents.hpp"

#include <cstdio>

#include <algorithm>


extern GameEvents* g_GameEvents;


static constexpr int32_t k_ExcludedGameEventIDs[] =
{
    53,
    70,
    72, // near miss chain in progeress
    75, // drifting
    76, // spinning
    77, // in air
    78, // wheelie
    79, // wheelie oncoming
    82, // oncoming
    84, // reverse oncoming
    87,
    88, // traffic checking chain
    90,
    131,
    135,
    137,
    138,
};


GameEvents::GameEvents()
    :
    m_DetourProcessGameEvents(reinterpret_cast<void*>(0x00A254DD), 7, &GameEvents::DetourProcessGameEvents)
{
    m_DetourProcessGameEvents.Attach();
}

GameEvents::~GameEvents()
{
    m_DetourProcessGameEvents.Detach();
}

void GameEvents::OnGameEvent(const uint8_t* eventData, int32_t eventID, uint32_t eventSize)
{
    if (std::binary_search(std::begin(k_ExcludedGameEventIDs), std::end(k_ExcludedGameEventIDs), eventID))
    {
        return;
    }

    printf_s("%4d  [%4X] ", eventID, eventSize);
    for (uint32_t i = 0; i < eventSize; ++i)
    {
        printf_s(" %02X", eventData[i]);
    }
    putchar('\n');
}

__declspec(naked) void GameEvents::DetourProcessGameEvents()
{
    __asm
    {
        pushfd
        pushad

        push dword ptr [esi - 0xC]
        push dword ptr [esi - 0x10]
        push esi
        mov ecx, dword ptr [g_GameEvents]
        call GameEvents::OnGameEvent

        popad
        popfd
        ret
    }
}
