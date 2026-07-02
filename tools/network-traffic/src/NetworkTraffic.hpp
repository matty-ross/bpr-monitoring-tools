#pragma once


#include "core/Logger.hpp"

#include "DummyServer.hpp"


class NetworkTraffic
{
public:
    static constexpr char k_Name[] = "Network Traffic";

private:
    NetworkTraffic();

public:
    static NetworkTraffic& Get();

public:
    void Load();
    void Unload();

private:
    static void HookEncryptData();
    static void HookDecryptData();

private:
    static NetworkTraffic s_Instance;

private:
    Core::Logger m_Logger;

    DummyServer m_DummyServer;
};
