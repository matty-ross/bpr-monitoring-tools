#include "NetworkTraffic.h"


NetworkTraffic::NetworkTraffic()
    :
    m_DummyServer("127.0.0.1", "16976"),
    m_DetourSend(reinterpret_cast<void*>(0x03A0CB87), 6, &NetworkTraffic::DetourSend),
    m_DetourRecv(reinterpret_cast<void*>(0x03A0D0B1), 5, &NetworkTraffic::DetourRecv)
{
    m_DetourSend.Attach();
    m_DetourRecv.Attach();
}

NetworkTraffic::~NetworkTraffic()
{
    m_DetourSend.Detach();
    m_DetourRecv.Detach();
}

void NetworkTraffic::OnSend(const void* data, size_t dataSize)
{
    m_DummyServer.OnSend(data, dataSize);
}

void NetworkTraffic::OnRecv(const void* data, size_t dataSize)
{
    m_DummyServer.OnRecv(data, dataSize);
}

__declspec(naked) void NetworkTraffic::DetourSend()
{
    __asm
    {
        pushfd
        pushad

        push dword ptr [ebp + 0x10]
        push dword ptr [ebp + 0xC]
        call NetworkTraffic::OnSend

        popad
        popfd
        ret
    }
}

__declspec(naked) void NetworkTraffic::DetourRecv()
{
    __asm
    {
        pushfd
        pushad

        push dword ptr [ebp + 0x14]
        push dword ptr [ebp + 0xC]
        call NetworkTraffic::OnRecv

        popad
        popfd
        ret
    }
}