#include "NetworkTraffic.h"


__declspec(naked) static void OnSendThunk()
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

__declspec(naked) static void OnRecvThunk()
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


NetworkTraffic::NetworkTraffic()
    :
    m_DummyServer("127.0.0.1", 16976),
    m_SendHook(reinterpret_cast<void*>(0x03A0CB87), 6, OnSendThunk),
    m_RecvHook(reinterpret_cast<void*>(0x03A0D0B1), 5, OnRecvThunk)
{
    s_Instance = this;

    m_SendHook.Attach();
    m_RecvHook.Attach();
}

NetworkTraffic::~NetworkTraffic()
{
    m_SendHook.Detach();
    m_RecvHook.Detach();
}

void __stdcall NetworkTraffic::OnSend(const uint8_t* data, size_t dataSize)
{
    s_Instance->m_DummyServer.OnSend(data, dataSize);
}

void __stdcall NetworkTraffic::OnRecv(const uint8_t* data, size_t dataSize)
{
    s_Instance->m_DummyServer.OnRecv(data, dataSize);
}