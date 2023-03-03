#include "GameActions.h"


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
    s_Instance = this;

    m_CheckGameActionsHook.Hook();
}

GameActions::~GameActions()
{
    m_CheckGameActionsHook.Unhook();
}

void __stdcall GameActions::OnCheckGameAction(const uint8_t* const actionData, const int32_t actionId, const uint32_t actionDataSize)
{
    s_Instance->m_Analyzer.OnCheckGameAction(actionData, actionId, actionDataSize);
}