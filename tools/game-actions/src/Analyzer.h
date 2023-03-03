#pragma once


#include <cstdint>


class Analyzer
{
public:
    void OnCheckGameAction(const uint8_t* const actionData, const int32_t actionId, const uint32_t actionDataSize) const;

private:
    bool IsGameActionIdExcluded(const int32_t actionId) const;
    void PrintGameAction(const uint8_t* const actionData, const int32_t actionId, const uint32_t actionDataSize) const;
};