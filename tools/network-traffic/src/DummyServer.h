#pragma once


#include <Windows.h>

#include "Socket.h"


class DummyServer
{
public:
    DummyServer(const char* host, uint16_t port);
    ~DummyServer();

    void OnSend(const uint8_t* data, size_t dataSize);
    void OnRecv(const uint8_t* data, size_t dataSize);

private:
    static DWORD CALLBACK SendThreadProc(LPVOID lpParameter);
    static DWORD CALLBACK RecvThreadProc(LPVOID lpParameter);

private:
    ClientSocket m_ClientSocket;
    ClientSocket m_ConnectedClientSocket;
    ServerSocket m_ServerSocket;
    
    HANDLE m_SendThread = nullptr;
    HANDLE m_RecvThread = nullptr;
    
    const uint8_t* m_RecvData = nullptr;
    size_t m_RecvDataSize = 0;
    bool m_NewRecvData = false;
    
    bool m_Running = true;
};