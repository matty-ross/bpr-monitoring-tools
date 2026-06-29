#include <cstddef>
//#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "core/Logger.hpp"

#include "DummyServer.hpp"


// https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock


DummyServer::DummyServer(int port, const Core::Logger& logger)
    :
    m_Port(port),
    m_Logger(logger)
{
}

void DummyServer::Load()
{
    // TODO: Move out of this class.
    //WSADATA wsaData = {};
    //int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    //if (result != 0)
    //{
    //    throw Core::WindowsException(
    //        HRESULT_FROM_WIN32(result),
    //        "Failed to initialize WinSock."
    //    );
    //}

    sockaddr_in address =
    {
        .sin_family = AF_INET,
        .sin_port = htons(m_Port),
        .sin_addr = INADDR_LOOPBACK,
    };

    m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(m_ServerSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    listen(m_ServerSocket, SOMAXCONN);

    m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(m_ClientSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));

    m_Running = true;

    auto serverHandlerThreadProc = [](LPVOID lpThreadParameter) -> DWORD
    {
        static_cast<DummyServer*>(lpThreadParameter)->ServerHandler();
        
        return 0;
    };
    m_ServerHandlerThreadHandle = CreateThread(nullptr, 0, serverHandlerThreadProc, this, 0, nullptr);
}

void DummyServer::Unload()
{
    m_Running = false;

    shutdown(m_ConnectedClientSocket, SD_BOTH);

    WaitForSingleObject(m_ServerHandlerThreadHandle, INFINITE);

    closesocket(m_ConnectedClientSocket);
    closesocket(m_ClientSocket);
    closesocket(m_ServerSocket);
    
    // TODO: Move out of this class.
    //WSACleanup();
}

void DummyServer::SendData(const void* data, size_t dataSize) const
{
    send(m_ClientSocket, static_cast<const char*>(data), dataSize, 0);
}

void DummyServer::ServerHandler()
{
    m_ConnectedClientSocket = accept(m_ServerSocket, nullptr, nullptr);

    while (m_Running)
    {
        char buffer[1024] = {};
        recv(m_ConnectedClientSocket, buffer, sizeof(buffer), 0);
    }
}
