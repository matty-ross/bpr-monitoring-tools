#pragma once


#include "lib/common/include/DetourHook.h"


class GameEvents
{
public:
    GameEvents();
    ~GameEvents();

public:
    static void __stdcall OnProcessGameEvent(const uint8_t* event, int32_t eventId, uint32_t eventSize);

private:
    DetourHook m_ProcessGameEventsHook;
};