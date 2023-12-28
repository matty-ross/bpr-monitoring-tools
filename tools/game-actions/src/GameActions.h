#pragma once


#include <cstdint>

#include "core/DetourHook.h"


class GameActions
{
public:
    GameActions();
    ~GameActions();

private:
    void OnGameAction(const uint8_t* actionData, int32_t actionID, uint32_t actionSize);

private:
    static void DetourCheckGameActions();

private:
    DetourHook m_DetourCheckGameActions;
};