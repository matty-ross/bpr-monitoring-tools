#include "Socket.h"

#include <WS2tcpip.h>


// https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
// https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code


static sockaddr GetAddress(const char* const host, const uint16_t port)
{
    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(address.sin_family, host, &address.sin_addr);

    return *reinterpret_cast<sockaddr*>(&address);
}


Socket::Socket()
    :
    m_Socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{
}

Socket::Socket(const SOCKET socket)
    :
    m_Socket(socket)
{
}

Socket::~Socket()
{
    if (m_Socket != INVALID_SOCKET)
    {
        closesocket(m_Socket);
    }
}


void ClientSocket::SetSocket(const SOCKET socket)
{
    m_Socket = socket;
}

bool ClientSocket::Connect(const char* const host, const uint16_t port) const
{
    const sockaddr address = GetAddress(host, port);
    const int result = connect(m_Socket, &address, sizeof(address));
    return result != SOCKET_ERROR;
}

bool ClientSocket::Shutdown() const
{
    const int result = shutdown(m_Socket, SD_BOTH);
    return result != SOCKET_ERROR;
}

bool ClientSocket::Send(const void* const data, const size_t dataSize) const
{
    const int result = send(m_Socket, static_cast<const char*>(data), dataSize, 0);
    return result != SOCKET_ERROR;
}

bool ClientSocket::Recv(void* const data, const size_t dataSize) const
{
    const int result = recv(m_Socket, static_cast<char*>(data), dataSize, 0);
    return result != 0 && result != SOCKET_ERROR;
}


bool ServerSocket::Bind(const char* const host, const uint16_t port) const
{
    const sockaddr address = GetAddress(host, port);
    const int result = bind(m_Socket, &address, sizeof(address));
    return result != SOCKET_ERROR;
}

bool ServerSocket::Listen() const
{
    const int result = listen(m_Socket, SOMAXCONN);
    return result != SOCKET_ERROR;
}

SOCKET ServerSocket::Accept() const
{
    return accept(m_Socket, nullptr, nullptr);
}