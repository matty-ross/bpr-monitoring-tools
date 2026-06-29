#pragma once


#include <cstddef>
//#include <Windows.h>
#include <WinSock2.h>

#include "core/Logger.hpp"


class DummyServer
{
public:
    DummyServer(int port, const Core::Logger& logger);

public:
    void Load();
    void Unload();

    void SendData(const void* data, size_t dataSize) const;

private:
    void ServerHandler();

private:
    int m_Port = 0;

    SOCKET m_ServerSocket = INVALID_SOCKET;
    SOCKET m_ClientSocket = INVALID_SOCKET;
    SOCKET m_ConnectedClientSocket = INVALID_SOCKET;

    HANDLE m_ServerHandlerThreadHandle = NULL;

    bool m_Running = false;

    const Core::Logger& m_Logger;
};
