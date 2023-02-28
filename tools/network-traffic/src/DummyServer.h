#pragma once


#include <thread>

#include "Socket.h"


class DummyServer
{
public:
    DummyServer(const char* const host, const uint16_t port);
    ~DummyServer();

    void OnSend(const uint8_t* const data, const size_t dataSize);
    void OnRecv(const uint8_t* const data, const size_t dataSize);

private:
    void SendThreadFn();
    void RecvThreadFn();

private:
    ClientSocket m_ClientSocket;
    ClientSocket m_ConnectedClientSocket;
    ServerSocket m_ServerSocket;
    std::thread m_SendThread;
    std::thread m_RecvThread;
    std::pair<const uint8_t*, size_t> m_RecvData;
    volatile bool m_NewRecvData = false;
    volatile bool m_Running = true;
};