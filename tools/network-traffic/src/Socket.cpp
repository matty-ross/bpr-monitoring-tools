#include "Socket.h"

#include <WS2tcpip.h>


// https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
// https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code


static sockaddr GetAddress(const char* host, uint16_t port)
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

Socket::Socket(SOCKET socket)
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


void ClientSocket::SetSocket(SOCKET socket)
{
    m_Socket = socket;
}

bool ClientSocket::Connect(const char* host, uint16_t port) const
{
    sockaddr address = GetAddress(host, port);
    int result = connect(m_Socket, &address, sizeof(address));
    return result != SOCKET_ERROR;
}

bool ClientSocket::Shutdown() const
{
    int result = shutdown(m_Socket, SD_BOTH);
    return result != SOCKET_ERROR;
}

bool ClientSocket::Send(const void* data, size_t dataSize) const
{
    int result = send(m_Socket, static_cast<const char*>(data), dataSize, 0);
    return result != SOCKET_ERROR;
}

bool ClientSocket::Recv(void* data, size_t dataSize) const
{
    int result = recv(m_Socket, static_cast<char*>(data), dataSize, 0);
    return result != 0 && result != SOCKET_ERROR;
}


bool ServerSocket::Bind(const char* host, uint16_t port) const
{
    sockaddr address = GetAddress(host, port);
    int result = bind(m_Socket, &address, sizeof(address));
    return result != SOCKET_ERROR;
}

bool ServerSocket::Listen() const
{
    int result = listen(m_Socket, SOMAXCONN);
    return result != SOCKET_ERROR;
}

SOCKET ServerSocket::Accept() const
{
    return accept(m_Socket, nullptr, nullptr);
}