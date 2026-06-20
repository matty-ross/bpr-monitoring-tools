#pragma once


#include <cstdint>

#include "core/Logger.hpp"


class GameEvents
{
private:
    GameEvents();

public:
    static GameEvents& Get();

public:
    void OnProcessAttach();
    void OnProcessDetach();

private:
    void PrintGameEvent(void* gameEvent, int32_t gameEventID, uint32_t gameEventSize) const;

private:
    static GameEvents s_Instance;

private:
    Core::Logger m_Logger;
};
