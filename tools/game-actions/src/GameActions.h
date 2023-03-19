#pragma once


#include "lib/common/src/DetourHook.h"

#include "Analyzer.h"


class GameActions
{
public:
    GameActions();
    ~GameActions();

    static void __stdcall OnCheckGameAction(const uint8_t* const actionData, const int32_t actionId, const uint32_t actionDataSize);

private:
    inline static GameActions* s_Instance = nullptr;
    Analyzer m_Analyzer;
    DetourHook m_CheckGameActionsHook;
};