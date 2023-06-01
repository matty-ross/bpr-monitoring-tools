#include "DummyServer.h"

#include <array>

#include <Windows.h>


DummyServer::DummyServer(const char* const host, const uint16_t port)
    :
    m_ConnectedClientSocket(INVALID_SOCKET)
{
    m_ServerSocket.Bind(host, port);
    m_ServerSocket.Listen();
    m_ClientSocket.Connect(host, port);
    
    const SOCKET client = m_ServerSocket.Accept();
    m_ConnectedClientSocket.SetSocket(client);

    m_SendThread = CreateThread(nullptr, 0, &DummyServer::SendThreadProc, this, 0, nullptr);
    m_RecvThread = CreateThread(nullptr, 0, &DummyServer::RecvThreadProc, this, 0, nullptr);
}

DummyServer::~DummyServer()
{
    m_Running = false;
    m_ClientSocket.Shutdown();

    const HANDLE threads[] = { m_SendThread, m_RecvThread };
    WaitForMultipleObjects(std::size(threads), threads, TRUE, INFINITE);

    CloseHandle(m_RecvThread);
    CloseHandle(m_SendThread);
}

void DummyServer::OnSend(const uint8_t* const data, const size_t dataSize)
{
    m_ClientSocket.Send(data, dataSize);
}

void DummyServer::OnRecv(const uint8_t* const data, const size_t dataSize)
{
    m_RecvData = { data, dataSize };
    m_NewRecvData = true;

    std::array<uint8_t, 1024> buffer = {};
    m_ClientSocket.Recv(buffer.data(), buffer.size());
}

DWORD CALLBACK DummyServer::SendThreadProc(const LPVOID lpParameter)
{
    DummyServer& ds = *static_cast<DummyServer*>(lpParameter);
    
    while (ds.m_Running)
    {
        if (ds.m_NewRecvData)
        {
            ds.m_NewRecvData = false;
            ds.m_ConnectedClientSocket.Send(ds.m_RecvData.first, ds.m_RecvData.second);
        }
    }
    
    return 0;
}

DWORD CALLBACK DummyServer::RecvThreadProc(const LPVOID lpParameter)
{
    DummyServer& ds = *static_cast<DummyServer*>(lpParameter);
    
    while (ds.m_Running)
    {
        std::array<uint8_t, 1024> buffer = {};
        ds.m_ConnectedClientSocket.Recv(buffer.data(), buffer.size());
    }
    
    return 0;
}