#pragma once


#include <cstddef>
#include <cstdint>

#include "core/Logger.hpp"


class GameEvents
{
private:
    GameEvents();

public:
    static GameEvents& Get();

public:
    void Load();

private:
    void PrintGameEvent(const std::byte* gameEvent, int32_t gameEventID, int32_t gameEventSize) const;

private:
    static void Hook_PrintGameEvent();

private:
    static constexpr char k_Name[] = "Game Events";

    static GameEvents s_Instance;

private:
    Core::Logger m_Logger;
};
