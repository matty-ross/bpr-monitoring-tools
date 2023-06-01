#include "GameActions.h"

#include <cstdio>

#include <algorithm>


static constexpr int32_t k_ExcludedGameActionIds[] =
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


static __declspec(naked) void OnCheckGameActionThunk()
{
    __asm
    {
        pushfd
        pushad

        push dword ptr [ecx - 0xC]
        push dword ptr [ecx - 0x10]
        push ecx
        call GameActions::OnCheckGameAction

        popad
        popfd
        ret
    }
}


GameActions::GameActions()
    :
    m_CheckGameActionsHook(reinterpret_cast<void*>(0x07050A59), 7, OnCheckGameActionThunk)
{
    m_CheckGameActionsHook.Attach();
}

GameActions::~GameActions()
{
    m_CheckGameActionsHook.Detach();
}

void __stdcall GameActions::OnCheckGameAction(const uint8_t* const action, const int32_t actionId, const uint32_t actionSize)
{
    if (std::binary_search(std::begin(k_ExcludedGameActionIds), std::end(k_ExcludedGameActionIds), actionId))
    {
        return;
    }

    printf_s("%4d  [%4X] ", actionId, actionSize);
    for (uint32_t i = 0; i < actionSize; ++i)
    {
        printf_s(" %02X", action[i]);
    }
    putchar('\n');
}