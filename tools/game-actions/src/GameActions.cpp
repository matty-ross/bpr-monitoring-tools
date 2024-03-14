#include "GameActions.hpp"

#include <cstdio>

#include <algorithm>


extern GameActions* g_GameActions;


static constexpr int32_t k_ExcludedGameActionIDs[] =
{
    15, // completed stunt
    16, // in progress stunt
    93,
    122, // traffic checking chain
    123, // trigger
    124, // killzone
    144,
    159,
    168,
    197,
    201, // drifting
    202, // spinning
    203, // in air
    204, // wheelie
    205, // oncoming
    206, // tailgating
    208,
    209, // event state response
    210,
    218,
    225,
    227,
    235,
    253,
    298,
    301,
    304,
    348,
    353,
};


GameActions::GameActions()
    :
    m_DetourCheckGameActions(reinterpret_cast<void*>(0x07050A59), 7, &GameActions::DetourCheckGameActions)
{
    m_DetourCheckGameActions.Attach();
}

GameActions::~GameActions()
{
    m_DetourCheckGameActions.Detach();
}

void GameActions::OnGameAction(const uint8_t* actionData, int32_t actionID, uint32_t actionSize)
{
    if (std::binary_search(std::begin(k_ExcludedGameActionIDs), std::end(k_ExcludedGameActionIDs), actionID))
    {
        return;
    }

    printf_s("%4d  [%4X] ", actionID, actionSize);
    for (uint32_t i = 0; i < actionSize; ++i)
    {
        printf_s(" %02X", actionData[i]);
    }
    putchar('\n');
}

__declspec(naked) void GameActions::DetourCheckGameActions()
{
    __asm
    {
        pushfd
        pushad

        push dword ptr [ecx - 0xC]
        push dword ptr [ecx - 0x10]
        push ecx
        mov ecx, dword ptr [g_GameActions]
        call GameActions::OnGameAction

        popad
        popfd
        ret
    }
}
