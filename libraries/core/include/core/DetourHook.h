#pragma once


#include <cstdint>


class DetourHook
{
public:
    DetourHook(void* hookAddress, size_t hookSize, const void* detourFunction);
    ~DetourHook();

    void Attach() const;
    void Detach() const;

private:
    void BuildDetourThunk(const void* detourFunction);

private:
    uint8_t* m_HookAddress = nullptr;
    size_t m_HookSize = 0;
    uint8_t* m_OriginalCode = nullptr;
    uint8_t* m_DetourThunk = nullptr;
};