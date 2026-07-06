#pragma once


#include "core/Logger.hpp"

#include "DummyServer.hpp"


class NetworkTraffic
{
private:
    NetworkTraffic();

public:
    static NetworkTraffic& Get();

public:
    void Load();
    void Unload();

private:
    static void Hook_ClientSendData();
    static void Hook_ServerSendData();

private:
    static constexpr char k_Name[] = "Network Traffic";

    static NetworkTraffic s_Instance;

private:
    Core::Logger m_Logger;

    DummyServer m_DummyServer;
};
