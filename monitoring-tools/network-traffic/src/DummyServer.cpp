#include <Windows.h>
#include <WinSock2.h>

#include "core/WindowsException.hpp"
#include "core/Logger.hpp"
#include "core/Patch.hpp"

#include "NetworkTraffic.hpp"
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
    m_Logger.Info("Server is listening.");

    m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(m_ClientSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    m_Logger.Info("Client connected to server.");

    m_ConnectedClientSocket = accept(serverSocket, nullptr, nullptr);
    closesocket(serverSocket);
    m_Logger.Info("Server accepted the client connection.");

    Core::Patch(0x0055F543, 6, m_Logger).WriteJMP(Hook_ClientSendData);
    Core::Patch(0x0055F255, 6, m_Logger).WriteJMP(Hook_ServerSendData);
}

void DummyServer::Unload()
{
    shutdown(m_ConnectedClientSocket, SD_BOTH);
    closesocket(m_ConnectedClientSocket);

    shutdown(m_ClientSocket, SD_BOTH);
    closesocket(m_ClientSocket);

    WSACleanup();
}

void DummyServer::ClientSendData(const void* data, int size) const
{
    send(m_ClientSocket, static_cast<const char*>(data), size, 0);

    int remainingRecvSize = size;
    while (remainingRecvSize > 0)
    {
        char buffer[1024] = {};
        remainingRecvSize -= recv(m_ConnectedClientSocket, buffer, sizeof(buffer), 0);
    }
}

void DummyServer::ServerSendData(const void* data, int size) const
{
    send(m_ConnectedClientSocket, static_cast<const char*>(data), size, 0);

    int remainingRecvSize = size;
    while (remainingRecvSize > 0)
    {
        char buffer[1024] = {};
        remainingRecvSize -= recv(m_ClientSocket, buffer, sizeof(buffer), 0);
    }
}

__declspec(naked) void DummyServer::Hook_ClientSendData()
{
    /*
        static int32_t _SendPacket(
            ProtoSSLRefT* pState,
            uint8_t uType,
            const void* pHeadPtr,
            int32_t iHeadLen,
            const void* pBodyPtr,
            int32_t iBodyLen
        )
    */

    __asm
    {
        // ebp + 0xC: uint8_t uType
        // ebp + 0x18: const void* pBodyPtr
        // ebp + 0x1C: int32_t iBodyLen

        pushfd
        pushad

        // uType == 23 (TLS application data)
        cmp byte ptr [ebp + 0xC], 23
        jne _end

        push dword ptr [ebp + 0x1C]
        push dword ptr [ebp + 0x18]
        mov ecx, offset NetworkTraffic::s_Instance.m_DummyServer
        call DummyServer::ClientSendData

    _end:
        popad
        popfd

        // Original code.
        mov ecx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebp + 0x8]

        // Jump back.
        push 0x0055F549
        ret
    }
}

__declspec(naked) void DummyServer::Hook_ServerSendData()
{
    /*
        static int32_t _RecvPacket(
            ProtoSSLRefT* pState
        )
    */

    __asm
    {
        // esi: SecureStateT* pSecure
        
        pushfd
        pushad

        mov eax, dword ptr [esi + 0x18] // int32_t pSecure->iRecvSize
        mov ebx, dword ptr [esi + 0x1C] // int32_t pSecure->iRecvBase
        lea ecx, [esi + 0xBD1C] // uint8_t* pSecure->RecvData

        // pSecure->RecvData[0] == 23 (TLS application data)
        cmp byte ptr [ecx + 0x0], 23
        jne _end

        // size = pSecure->iRecvSize - pSecure->iRecvBase
        sub eax, ebx

        // data = pSecure->RecvData + pSecure->iRecvBase
        add ecx, ebx

        push eax
        push ecx
        mov ecx, offset NetworkTraffic::s_Instance.m_DummyServer
        call DummyServer::ServerSendData

    _end:
        popad
        popfd

        // Original code.
        inc dword ptr [esi + 0x10]
        mov eax, dword ptr [esi + 0x18]

        // Jump back.
        push 0x0055F25B
        ret
    }
}
