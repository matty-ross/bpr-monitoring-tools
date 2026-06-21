#pragma once


#include <cstddef>
#include <cstdint>

#include "core/Logger.hpp"


class GameActions
{
private:
    GameActions();

public:
    static GameActions& Get();

public:
    void OnProcessAttach();
    void OnProcessDetach();

private:
    void PrintGameAction(const std::byte* gameAction, int32_t gameActionID, uint32_t gameActionSize) const;

private:
    static GameActions s_Instance;

private:
    Core::Logger m_Logger;
};
