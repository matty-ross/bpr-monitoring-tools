#pragma once


#include <WinSock2.h>

#include "core/Logger.hpp"


class DummyServer
{
public:
    DummyServer(const Core::Logger& logger);

public:
    void Load();
    void Unload();

    void ClientSendData(const void* data, int size) const;
    void ServerSendData(const void* data, int size) const;

private:
    static constexpr int k_Port = 40123;

private:
    SOCKET m_ClientSocket = INVALID_SOCKET;
    SOCKET m_ConnectedClientSocket = INVALID_SOCKET;

    const Core::Logger& m_Logger;
};
