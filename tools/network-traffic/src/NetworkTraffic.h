#pragma once


#include "WinsockManager.h"
#include "DummyServer.h"
#include "DetourHook.h"


class NetworkTraffic
{
public:
    NetworkTraffic();
    ~NetworkTraffic();

    static void __stdcall OnSend(const uint8_t* const data, const size_t dataSize);
    static void __stdcall OnRecv(const uint8_t* const data, const size_t dataSize);

private:
    inline static NetworkTraffic* s_Instance = nullptr;
    WinsockManager m_WinsockManager;
    DummyServer m_DummyServer;
    DetourHook m_SendHook;
    DetourHook m_RecvHook;
};