#include <cstddef>
#include <WinSock2.h>

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
    sockaddr_in address =
    {
        .sin_family = AF_INET,
        .sin_port = htons(k_Port),
        .sin_addr = INADDR_LOOPBACK,
    };

    m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(m_ServerSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    listen(m_ServerSocket, SOMAXCONN);

    m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(m_ClientSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));

    PTHREAD_START_ROUTINE serverHandlerThreadProc = [](LPVOID lpThreadParameter) -> DWORD
    {
        DummyServer& dummyServer = *static_cast<DummyServer*>(lpThreadParameter);
        dummyServer.ServerHandler();

        return 0;
    };
    m_ServerHandlerThreadHandle = CreateThread(nullptr, 0, serverHandlerThreadProc, this, 0, nullptr);
}

void DummyServer::Unload()
{
    shutdown(m_ConnectedClientSocket, SD_BOTH);
    shutdown(m_ClientSocket, SD_BOTH);

    WaitForSingleObject(m_ServerHandlerThreadHandle, INFINITE);
    CloseHandle(m_ServerHandlerThreadHandle);

    closesocket(m_ConnectedClientSocket);
    closesocket(m_ClientSocket);
    closesocket(m_ServerSocket);
}

void DummyServer::ClientSendData(const void* data, size_t dataSize) const
{
    send(m_ClientSocket, static_cast<const char*>(data), dataSize, 0);
}

void DummyServer::ServerSendData(const void* data, size_t dataSize) const
{
    send(m_ConnectedClientSocket, static_cast<const char*>(data), dataSize, 0);
}

void DummyServer::ServerHandler()
{
    m_ConnectedClientSocket = accept(m_ServerSocket, nullptr, nullptr);

    while (true)
    {
        char buffer[1024] = {};
        if (recv(m_ConnectedClientSocket, buffer, sizeof(buffer), 0) == 0)
        {
            break;
        }
    }
}
