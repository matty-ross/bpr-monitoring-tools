#include <array>
#include <algorithm>

#include "Analyzer.h"


static constexpr std::array<int32_t, 24> k_ExcludedGameActionIds =
{
    16,
    93,
    122, // traffic checking chain
    123, // trigger
    144,
    159,
    168,
    197,
    201, // drifting
    202, // spinning
    203, // in air
    204, // wheelie
    205, // oncoming
    206, // tailgating
    208,
    218,
    225,
    227,
    235,
    253,
    301,
    304,
    348,
    353,
};


void Analyzer::OnCheckGameAction(const uint8_t* const actionData, const int32_t actionId, const uint32_t actionDataSize) const
{
    if (IsGameActionIdExcluded(actionId))
    {
        return;
    }

    PrintGameAction(actionData, actionId, actionDataSize);
}

bool Analyzer::IsGameActionIdExcluded(const int32_t actionId) const
{
    const auto it = std::find(k_ExcludedGameActionIds.begin(), k_ExcludedGameActionIds.end(), actionId);
    return it != k_ExcludedGameActionIds.end();
}

void Analyzer::PrintGameAction(const uint8_t* const actionData, const int32_t actionId, const uint32_t actionDataSize) const
{
    printf_s("%4d  [%4X] ", actionId, actionDataSize);
    for (uint32_t i = 0; i < actionDataSize; ++i)
    {
        printf_s(" %02X", actionData[i]);
    }
    puts("\n------------------------------");
}