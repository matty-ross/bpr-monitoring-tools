#include "DummyServer.hpp"

#include <WS2tcpip.h>


// https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock


DummyServer::DummyServer(const char* host, const char* port)
{
    WSADATA wsaData = {};
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        MessageBoxA(nullptr, "Winsock failed to initialize!", "ERROR", MB_ICONERROR);
        return;
    }

    m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    addrinfo hints =
    {
        .ai_flags    = AI_PASSIVE,
        .ai_family   = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP,
    };
    addrinfo* addrInfo = nullptr;
    getaddrinfo(host, port, &hints, &addrInfo);

    bind(m_ServerSocket, addrInfo->ai_addr, addrInfo->ai_addrlen);
    listen(m_ServerSocket, SOMAXCONN);
    connect(m_ClientSocket, addrInfo->ai_addr, addrInfo->ai_addrlen);
    m_ConnectedClientSocket = accept(m_ServerSocket, nullptr, nullptr);
    
    freeaddrinfo(addrInfo);

    m_Running = true;

    m_SendThread = CreateThread(nullptr, 0, &DummyServer::SendThreadProc, this, 0, nullptr);
    m_RecvThread = CreateThread(nullptr, 0, &DummyServer::RecvThreadProc, this, 0, nullptr);
}

DummyServer::~DummyServer()
{
    m_Running = false;

    shutdown(m_ConnectedClientSocket, SD_BOTH);
    
    HANDLE threads[] = { m_SendThread, m_RecvThread };
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(m_RecvThread);
    CloseHandle(m_SendThread);

    closesocket(m_ConnectedClientSocket);
    closesocket(m_ClientSocket);
    closesocket(m_ServerSocket);

    WSACleanup();
}

void DummyServer::OnSend(const void* data, size_t dataSize)
{
    // Game sends data to the server.
    
    send(m_ClientSocket, static_cast<const char*>(data), dataSize, 0);
}

void DummyServer::OnRecv(const void* data, size_t dataSize)
{
    // Game receives data from the server.

    m_RecvData = data;
    m_RecvDataSize = dataSize;
    m_NewRecvData = true;

    char buffer[1024] = {};
    recv(m_ClientSocket, buffer, sizeof(buffer), 0);
}

DWORD CALLBACK DummyServer::SendThreadProc(LPVOID lpParameter)
{
    DummyServer& ds = *static_cast<DummyServer*>(lpParameter);
    
    while (ds.m_Running)
    {
        if (ds.m_NewRecvData)
        {
            ds.m_NewRecvData = false;
            send(ds.m_ConnectedClientSocket, static_cast<const char*>(ds.m_RecvData), ds.m_RecvDataSize, 0);
        }
    }
    
    return 0;
}

DWORD CALLBACK DummyServer::RecvThreadProc(LPVOID lpParameter)
{
    DummyServer& ds = *static_cast<DummyServer*>(lpParameter);
    
    while (ds.m_Running)
    {
        char buffer[1024] = {};
        recv(ds.m_ConnectedClientSocket, buffer, sizeof(buffer), 0);
    }
    
    return 0;
}
