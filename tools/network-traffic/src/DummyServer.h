#pragma once


#include <cstdint>

#include <Windows.h>
#include <WinSock2.h>


class DummyServer
{
public:
    DummyServer(const char* host, const char* port);
    ~DummyServer();

public:
    void OnSend(const void* data, size_t dataSize);
    void OnRecv(const void* data, size_t dataSize);

private:
    static DWORD CALLBACK SendThreadProc(LPVOID lpParameter);
    static DWORD CALLBACK RecvThreadProc(LPVOID lpParameter);

private:
    SOCKET m_ClientSocket = INVALID_SOCKET;
    SOCKET m_ServerSocket = INVALID_SOCKET;
    SOCKET m_ConnectedClientSocket = INVALID_SOCKET;
    
    HANDLE m_SendThread = nullptr;
    HANDLE m_RecvThread = nullptr;
    
    const void* m_RecvData = nullptr;
    size_t m_RecvDataSize = 0;
    bool m_NewRecvData = false;
    
    bool m_Running = false;
};