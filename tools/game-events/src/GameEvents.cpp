#include "GameEvents.h"

#include <cstdio>

#include <algorithm>


static constexpr int32_t k_ExcludedGameEventIds[] =
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


static __declspec(naked) void OnProcessGameEventThunk()
{
    __asm
    {
        pushfd
        pushad

        push dword ptr [esi - 0xC]
        push dword ptr [esi - 0x10]
        push esi
        call GameEvents::OnProcessGameEvent

        popad
        popfd
        ret
    }
}


GameEvents::GameEvents()
    :
    m_ProcessGameEventsHook(reinterpret_cast<void*>(0x00A254DD), 7, OnProcessGameEventThunk)
{
    m_ProcessGameEventsHook.Attach();
}

GameEvents::~GameEvents()
{
    m_ProcessGameEventsHook.Detach();
}

void __stdcall GameEvents::OnProcessGameEvent(const uint8_t* const event, const int32_t eventId, const uint32_t eventSize)
{
    if (std::binary_search(std::begin(k_ExcludedGameEventIds), std::end(k_ExcludedGameEventIds), eventId))
    {
        return;
    }

    printf_s("%4d  [%4X] ", eventId, eventSize);
    for (uint32_t i = 0; i < eventSize; ++i)
    {
        printf_s(" %02X", event[i]);
    }
    putchar('\n');
}