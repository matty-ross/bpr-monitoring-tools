#pragma once


#include "lib/common/include/DetourHook.h"


class GameEvents
{
public:
    GameEvents();
    ~GameEvents();

public:
    static void __stdcall OnProcessGameEvent(const uint8_t* const event, const int32_t eventId, const uint32_t eventSize);

private:
    DetourHook m_ProcessGameEventsHook;
};