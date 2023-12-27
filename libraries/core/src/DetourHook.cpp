#include "DetourHook.h"

#include <Windows.h>


static int32_t GetDisplacement(const void* nextInstruction, const void* destination)
{
    return reinterpret_cast<uintptr_t>(destination) - reinterpret_cast<uintptr_t>(nextInstruction);
}


DetourHook::DetourHook(void* hookAddress, size_t hookSize, const void* detourFunction)
    :
    m_HookAddress(static_cast<uint8_t*>(hookAddress)),
    m_HookSize(hookSize)
{
    m_OriginalCode = new uint8_t[m_HookSize];
    memcpy_s(m_OriginalCode, m_HookSize, m_HookAddress, m_HookSize);
    BuildDetourThunk(detourFunction);
}

DetourHook::~DetourHook()
{
    VirtualFree(m_DetourThunk, 0, MEM_RELEASE);
    delete[] m_OriginalCode;
}

void DetourHook::Attach() const
{
    DWORD oldProtection = 0;
    VirtualProtect(m_HookAddress, m_HookSize, PAGE_EXECUTE_READWRITE, &oldProtection);

    *reinterpret_cast<uint8_t*>(m_HookAddress + 0x0) = 0xE9;
    *reinterpret_cast<int32_t*>(m_HookAddress + 0x1) = GetDisplacement(m_HookAddress + 0x5, m_DetourThunk);
    memset(m_HookAddress + 0x5, 0x90, m_HookSize - 5);

    VirtualProtect(m_HookAddress, m_HookSize, oldProtection, &oldProtection);
}

void DetourHook::Detach() const
{
    DWORD oldProtection = 0;
    VirtualProtect(m_HookAddress, m_HookSize, PAGE_EXECUTE_READWRITE, &oldProtection);

    memcpy_s(m_HookAddress, m_HookSize, m_OriginalCode, m_HookSize);

    VirtualProtect(m_HookAddress, m_HookSize, oldProtection, &oldProtection);
}

void DetourHook::BuildDetourThunk(const void* detourFunction)
{
    size_t detourThunkSize = 5 + m_HookSize + 5;
    
    m_DetourThunk = static_cast<uint8_t*>(VirtualAlloc(nullptr, detourThunkSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
    if (m_DetourThunk == nullptr)
    {
        return;
    }

    ptrdiff_t offset = 0x0;

    // Call the detour function.
    {
        *reinterpret_cast<uint8_t*>(m_DetourThunk + offset + 0x0) = 0xE8;
        *reinterpret_cast<int32_t*>(m_DetourThunk + offset + 0x1) = GetDisplacement(m_DetourThunk + offset + 0x5, detourFunction);
        offset += 5;
    }

    // Original code.
    {
        memcpy_s(m_DetourThunk + offset, m_HookSize, m_OriginalCode, m_HookSize);
        offset += m_HookSize;
    }

    // Jump back to the original code.
    {
        *reinterpret_cast<uint8_t*>(m_DetourThunk + offset + 0x0) = 0xE9;
        *reinterpret_cast<int32_t*>(m_DetourThunk + offset + 0x1) = GetDisplacement(m_DetourThunk + offset + 0x5, m_HookAddress + m_HookSize);
        offset += 5;
    }

    DWORD oldProtection = 0;
    VirtualProtect(m_DetourThunk, detourThunkSize, PAGE_EXECUTE_READ, &oldProtection);
}