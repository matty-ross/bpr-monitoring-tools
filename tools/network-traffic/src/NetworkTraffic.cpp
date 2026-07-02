#include <exception>
#include <Windows.h>

#include "core/Logger.hpp"
#include "core/Patch.hpp"

#include "NetworkTraffic.hpp"


NetworkTraffic NetworkTraffic::s_Instance;


NetworkTraffic::NetworkTraffic()
    :
    m_Logger(k_Name),
    m_DummyServer(m_Logger)
{
}

NetworkTraffic& NetworkTraffic::Get()
{
    return s_Instance;
}

void NetworkTraffic::Load()
{
    try
    {
        Core::Logger::Initialize();

        m_DummyServer.Load();

        Core::Patch(0x03A0CB84, 6, m_Logger).WriteJMP(HookEncryptData);
        Core::Patch(0x03A0D0B1, 5, m_Logger).WriteJMP(HookDecryptData);
    }
    catch (const std::exception& ex)
    {
        m_Logger.Error("%s", ex.what());
        MessageBoxA(NULL, ex.what(), k_Name, MB_ICONERROR);
    }
}

void NetworkTraffic::Unload()
{
    try
    {
        m_DummyServer.Unload();
    }
    catch (const std::exception& ex)
    {
        m_Logger.Error("%s", ex.what());
        MessageBoxA(NULL, ex.what(), k_Name, MB_ICONERROR);
    }
}

__declspec(naked) void NetworkTraffic::HookEncryptData()
{
    __asm
    {
        pushfd
        pushad

        push dword ptr [ebp + 0x10]
        push dword ptr [ebp + 0xC]
        mov ecx, offset NetworkTraffic::s_Instance.m_DummyServer
        call DummyServer::ClientSendData

        popad
        popfd

        // Original code.
        push dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebp + 0xC]

        push 0x03A0CB8A
        ret
    }
}

__declspec(naked) void NetworkTraffic::HookDecryptData()
{
    __asm
    {
        pushfd
        pushad

        push dword ptr [ebp + 0x14]
        push dword ptr [ebp + 0xC]
        mov ecx, offset NetworkTraffic::s_Instance.m_DummyServer
        call DummyServer::ServerSendData

        popad
        popfd

        // Original code.
        add esp, 0x10
        test eax, eax

        push 0x03A0D0B6
        ret
    }
}
