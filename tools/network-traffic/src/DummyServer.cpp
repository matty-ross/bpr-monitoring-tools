#include <array>

#include <Windows.h>

#include "DummyServer.h"


DummyServer::DummyServer(const char* const host, const uint16_t port)
    :
    m_ConnectedClientSocket(INVALID_SOCKET)
{
    m_ServerSocket.Bind(host, port);
    m_ServerSocket.Listen();
    m_ClientSocket.Connect(host, port);
    
    const SOCKET client = m_ServerSocket.Accept();
    m_ConnectedClientSocket.SetSocket(client);

    m_SendThread = std::thread(&DummyServer::SendThreadFn, this);
    m_RecvThread = std::thread(&DummyServer::RecvThreadFn, this);
}

DummyServer::~DummyServer()
{
    m_Running = false;
    m_ClientSocket.Shutdown();

    m_SendThread.join();
    m_RecvThread.join();
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

void DummyServer::SendThreadFn()
{
    while (m_Running)
    {
        if (m_NewRecvData)
        {
            m_NewRecvData = false;
            m_ConnectedClientSocket.Send(m_RecvData.first, m_RecvData.second);
        }
    }
}

void DummyServer::RecvThreadFn()
{
    while (m_Running)
    {
        std::array<uint8_t, 1024> buffer = {};
        m_ConnectedClientSocket.Recv(buffer.data(), buffer.size());
    }
}