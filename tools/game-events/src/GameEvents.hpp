#pragma once


#include <cstddef>
#include <cstdint>

#include "core/Logger.hpp"


class GameEvents
{
public:
    static constexpr char k_Name[] = "Game Events";

private:
    GameEvents();

public:
    static GameEvents& Get();

public:
    void Load();

private:
    static void HookPrintGameEvent();

private:
    void PrintGameEvent(const std::byte* gameEvent, int32_t gameEventID, uint32_t gameEventSize) const;

private:
    static GameEvents s_Instance;

private:
    Core::Logger m_Logger;
};
