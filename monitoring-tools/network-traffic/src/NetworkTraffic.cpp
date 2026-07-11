#include <exception>
#include <Windows.h>

#include "core/Logger.hpp"
#include "core/Patch.hpp"

#include "NetworkTraffic.hpp"
#include "DummyServer.hpp"


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

        Core::Patch(0x0055F543, 6, m_Logger).WriteJMP(Hook_ClientSendData);
        Core::Patch(0x0055F255, 6, m_Logger).WriteJMP(Hook_ServerSendData);
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

__declspec(naked) void NetworkTraffic::Hook_ClientSendData()
{
    /*
        int32_t _SendPacket(
            ProtoSSLRefT* pState,
            uint8_t uType,
            const void* pHeadPtr,
            int32_t iHeadLen,
            const void* pBodyPtr,
            int32_t iBodyLen
        )
    */

    __asm
    {
        pushfd
        pushad

        // uType == 23 (TLS application data)
        cmp byte ptr [ebp + 0xC], 23
        jne _end

        push dword ptr [ebp + 0x1C] // int32_t iBodyLen
        push dword ptr [ebp + 0x18] // void* pBodyPtr
        mov ecx, offset NetworkTraffic::s_Instance.m_DummyServer
        call DummyServer::ClientSendData

    _end:
        popad
        popfd

        // Original code.
        mov ecx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebp + 0x8]

        // Jump back.
        push 0x0055F549
        ret
    }
}

__declspec(naked) void NetworkTraffic::Hook_ServerSendData()
{
    /*
        int32_t _RecvPacket(
            ProtoSSLRefT* pState
        )
    */

    __asm
    {
        pushfd
        pushad

        // esi: SecureStateT* pSecure

        mov eax, dword ptr [esi + 0x18] // int32_t pSecure->iRecvSize
        mov ebx, dword ptr [esi + 0x1C] // int32_t pSecure->iRecvBase
        lea ecx, [esi + 0xBD1C] // uint8_t* pSecure->RecvData

        // pSecure->RecvData[0] == 23 (TLS application data)
        cmp byte ptr [ecx + 0x0], 23
        jne _end

        // size = pSecure->iRecvSize - pSecure->iRecvBase
        sub eax, ebx

        // data = pSecure->RecvData + pSecure->iRecvBase
        add ecx, ebx

        push eax
        push ecx
        mov ecx, offset NetworkTraffic::s_Instance.m_DummyServer
        call DummyServer::ServerSendData

    _end:
        popad
        popfd

        // Original code.
        inc dword ptr [esi + 0x10]
        mov eax, dword ptr [esi + 0x18]

        // Jump back.
        push 0x0055F25B
        ret
    }
}
