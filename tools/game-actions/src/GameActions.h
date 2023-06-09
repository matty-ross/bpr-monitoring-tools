#pragma once


#include "lib/common/include/DetourHook.h"


class GameActions
{
public:
    GameActions();
    ~GameActions();

public:
    static void __stdcall OnCheckGameAction(const uint8_t* action, int32_t actionId, uint32_t actionSize);

private:
    DetourHook m_CheckGameActionsHook;
};