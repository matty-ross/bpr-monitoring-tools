#include "NetworkTraffic.h"


static __declspec(naked) void OnSendThunk()
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

static __declspec(naked) void OnRecvThunk()
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
    m_DummyServer("127.0.0.1", 0x4250), // ascii 'BP'
    m_SendHook(reinterpret_cast<void*>(0x03A0CB87), 6, OnSendThunk),
    m_RecvHook(reinterpret_cast<void*>(0x03A0D0B1), 5, OnRecvThunk)
{
    s_Instance = this;

    m_SendHook.Hook();
    m_RecvHook.Hook();
}

NetworkTraffic::~NetworkTraffic()
{
    m_SendHook.Unhook();
    m_RecvHook.Unhook();
}

void __stdcall NetworkTraffic::OnSend(const uint8_t* const data, const size_t dataSize)
{
    s_Instance->m_DummyServer.OnSend(data, dataSize);
}

void __stdcall NetworkTraffic::OnRecv(const uint8_t* const data, const size_t dataSize)
{
    s_Instance->m_DummyServer.OnRecv(data, dataSize);
}