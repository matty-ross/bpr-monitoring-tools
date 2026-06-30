#pragma once


#include <cstddef>
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

    void ClientSendData(const void* data, size_t dataSize) const;
    void ServerSendData(const void* data, size_t dataSize) const;

private:
    void ServerHandler();

private:
    SOCKET m_ServerSocket = INVALID_SOCKET;
    SOCKET m_ClientSocket = INVALID_SOCKET;
    SOCKET m_ConnectedClientSocket = INVALID_SOCKET;

    HANDLE m_ServerHandlerThreadHandle = NULL;

    const Core::Logger& m_Logger;
};
