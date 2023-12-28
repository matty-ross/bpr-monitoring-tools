#pragma once


#include "core/DetourHook.h"

#include "DummyServer.h"


class NetworkTraffic
{
public:
    NetworkTraffic();
    ~NetworkTraffic();

private:
    void OnSend(const void* data, size_t dataSize);
    void OnRecv(const void* data, size_t dataSize);

private:
    static void DetourSend();
    static void DetourRecv();

private:
    DummyServer m_DummyServer;
    
    DetourHook m_DetourSend;
    DetourHook m_DetourRecv;
};