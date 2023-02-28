#include <array>

#include <Windows.h>

#include "DetourHook.h"


static uint32_t CalculateDisplacementOffset(const void* const currentAddress, const void* const destinationAddress)
{
    return static_cast<const uint8_t*>(destinationAddress) - static_cast<const uint8_t*>(currentAddress) - 0x5;
}


DetourHook::DetourHook(void* const hookAddress, const size_t hookSize, const void* const detourFunction)
    :
    m_HookAddress(static_cast<uint8_t*>(hookAddress)),
    m_HookSize(hookSize),
    m_OriginalCode(new uint8_t[hookSize])
{
    memcpy_s(m_OriginalCode, m_HookSize, m_HookAddress, m_HookSize);
    BuildDetourThunk(detourFunction);
}

DetourHook::~DetourHook()
{
    ::VirtualFree(m_DetourThunk, 0, MEM_RELEASE);
    delete[] m_OriginalCode;
}

void DetourHook::Hook() const
{
    DWORD oldProtection = 0;
    ::VirtualProtect(m_HookAddress, m_HookSize, PAGE_EXECUTE_READWRITE, &oldProtection);

    {
        static constexpr std::array<uint8_t, 5> jmpOpcode =
        {
            0xE9, 0x00, 0x00, 0x00, 0x00
        };

        memcpy_s(m_HookAddress, m_HookSize, jmpOpcode.data(), jmpOpcode.size());
        *reinterpret_cast<uint32_t*>(m_HookAddress + 0x1) = CalculateDisplacementOffset(m_HookAddress, m_DetourThunk);
        memset(m_HookAddress + 0x5, 0x90, m_HookSize - jmpOpcode.size());
    }

    ::VirtualProtect(m_HookAddress, m_HookSize, oldProtection, &oldProtection);
}

void DetourHook::Unhook() const
{
    DWORD oldProtection = 0;
    ::VirtualProtect(m_HookAddress, m_HookSize, PAGE_EXECUTE_READWRITE, &oldProtection);

    {
        memcpy_s(m_HookAddress, m_HookSize, m_OriginalCode, m_HookSize);
    }

    ::VirtualProtect(m_HookAddress, m_HookSize, oldProtection, &oldProtection);
}

void DetourHook::BuildDetourThunk(const void* const detourFunction)
{
    static constexpr std::array<uint8_t, 5> callOpcode =
    {
        0xE8, 0x00, 0x00, 0x00, 0x00
    };

    static constexpr std::array<uint8_t, 5> jmpOpcode =
    {
        0xE9, 0x00, 0x00, 0x00, 0x00
    };

    const size_t detourThunkSize = callOpcode.size() + m_HookSize + jmpOpcode.size();
    m_DetourThunk = static_cast<uint8_t*>(::VirtualAlloc(nullptr, detourThunkSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
    if (m_DetourThunk == nullptr)
    {
        return;
    }

    uint8_t* currentAddress = m_DetourThunk;
    size_t remainingSize = detourThunkSize;

    // call detour function
    {
        memcpy_s(currentAddress, remainingSize, callOpcode.data(), callOpcode.size());
        *reinterpret_cast<uint32_t*>(currentAddress + 0x1) = CalculateDisplacementOffset(currentAddress, detourFunction);

        currentAddress += callOpcode.size();
        remainingSize -= callOpcode.size();
    }

    // original code
    {
        memcpy_s(currentAddress, remainingSize, m_OriginalCode, m_HookSize);

        currentAddress += m_HookSize;
        remainingSize -= m_HookSize;
    }

    // jump back to the original code
    {
        memcpy_s(currentAddress, remainingSize, jmpOpcode.data(), jmpOpcode.size());
        *reinterpret_cast<uint32_t*>(currentAddress + 0x1) = CalculateDisplacementOffset(currentAddress, m_HookAddress + m_HookSize);

        currentAddress += jmpOpcode.size();
        remainingSize -= jmpOpcode.size();
    }

    DWORD oldProtection = 0;
    ::VirtualProtect(m_DetourThunk, detourThunkSize, PAGE_EXECUTE_READ, &oldProtection);
}