#pragma comment(lib, "Ws2_32.lib")


#include <Windows.h>
#include <WinSock2.h>

#include "core/WindowsException.hpp"
#include "core/Logger.hpp"

#include "DummyServer.hpp"


// https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock


DummyServer::DummyServer(const Core::Logger& logger)
    :
    m_Logger(logger)
{
}

void DummyServer::Load()
{
    WSADATA wsaData = {};
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        throw Core::WindowsException(
            HRESULT_FROM_WIN32(result),
            "Failed to initialize WinSock."
        );
    }

    sockaddr_in address =
    {
        .sin_family = AF_INET,
        .sin_port = htons(k_Port),
    };
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(serverSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    listen(serverSocket, SOMAXCONN);
    m_Logger.Info("Created server socket.");

    m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(m_ClientSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    m_Logger.Info("Created client socket.");

    m_ConnectedClientSocket = accept(serverSocket, nullptr, nullptr);
    closesocket(serverSocket);
    m_Logger.Info("Connected the server and client sockets.");
}

void DummyServer::Unload()
{
    shutdown(m_ConnectedClientSocket, SD_BOTH);
    closesocket(m_ConnectedClientSocket);
    
    shutdown(m_ClientSocket, SD_BOTH);
    closesocket(m_ClientSocket);

    WSACleanup();
}

void DummyServer::ClientSendData(const void* data, int dataSize) const
{
    send(m_ClientSocket, static_cast<const char*>(data), dataSize, 0);

    int remainingDataSize = dataSize;
    while (remainingDataSize > 0)
    {
        char buffer[1024] = {};
        remainingDataSize -= recv(m_ConnectedClientSocket, buffer, sizeof(buffer), 0);
    }
}

void DummyServer::ServerSendData(const void* data, int dataSize) const
{
    send(m_ConnectedClientSocket, static_cast<const char*>(data), dataSize, 0);

    int remainingDataSize = dataSize;
    while (remainingDataSize > 0)
    {
        char buffer[1024] = {};
        remainingDataSize -= recv(m_ClientSocket, buffer, sizeof(buffer), 0);
    }
}
