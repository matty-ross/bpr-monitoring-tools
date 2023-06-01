#pragma once


#include <utility>

#include <Windows.h>

#include "Socket.h"


class DummyServer
{
public:
    DummyServer(const char* const host, const uint16_t port);
    ~DummyServer();

    void OnSend(const uint8_t* const data, const size_t dataSize);
    void OnRecv(const uint8_t* const data, const size_t dataSize);

private:
    static DWORD CALLBACK SendThreadProc(const LPVOID lpParameter);
    static DWORD CALLBACK RecvThreadProc(const LPVOID lpParameter);

private:
    ClientSocket m_ClientSocket;
    ClientSocket m_ConnectedClientSocket;
    ServerSocket m_ServerSocket;
    
    HANDLE m_SendThread = nullptr;
    HANDLE m_RecvThread = nullptr;
    
    std::pair<const uint8_t*, size_t> m_RecvData;
    bool m_NewRecvData = false;
    
    bool m_Running = true;
};