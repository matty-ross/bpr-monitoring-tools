#pragma once


#include <cstddef>
#include <cstdint>

#include "core/Logger.hpp"


class GameActions
{
public:
    static constexpr char k_Name[] = "Game Actions";

private:
    GameActions();

public:
    static GameActions& Get();

public:
    void Load();

private:
    static void HookPrintGameAction();

private:
    void PrintGameAction(const std::byte* gameAction, int32_t gameActionID, uint32_t gameActionSize) const;

private:
    static GameActions s_Instance;

private:
    Core::Logger m_Logger;
};
