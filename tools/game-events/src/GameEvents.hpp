#pragma once


#include <cstdint>

#include "core/DetourHook.hpp"


class GameEvents
{
public:
    GameEvents();
    ~GameEvents();

private:
    void OnGameEvent(const uint8_t* eventData, int32_t eventID, uint32_t eventSize);

private:
    static void DetourProcessGameEvents();

private:
    DetourHook m_DetourProcessGameEvents;
};
