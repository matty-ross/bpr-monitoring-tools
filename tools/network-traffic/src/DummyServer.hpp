#pragma once


#include <WinSock2.h>

#include "core/Logger.hpp"


class DummyServer
{
public:
    static constexpr int k_Port = 40123;

public:
    DummyServer(const Core::Logger& logger);

public:
    void Load();
    void Unload();

    void ClientSendData(const void* data, int dataSize) const;
    void ServerSendData(const void* data, int dataSize) const;

private:
    SOCKET m_ServerSocket = INVALID_SOCKET;
    SOCKET m_ClientSocket = INVALID_SOCKET;
    SOCKET m_ConnectedClientSocket = INVALID_SOCKET;

    const Core::Logger& m_Logger;
};
