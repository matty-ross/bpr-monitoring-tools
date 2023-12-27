#pragma once


#include <cstdint>

#include "core/DetourHook.h"


class GameEvents
{
public:
    GameEvents();
    ~GameEvents();

private:
    void OnGameEvent(const uint8_t* eventData, int32_t eventID, uint32_t eventSize);

private:
    static void DetourOnProcessGameEvent();

private:
    DetourHook m_DetourOnProcessGameEvent;
};