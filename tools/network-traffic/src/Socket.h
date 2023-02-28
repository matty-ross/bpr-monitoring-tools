#pragma once


#include <cstdint>

#include <Windows.h>
#include <WinSock2.h>


class Socket
{
public:
    Socket();
    Socket(const SOCKET socket);
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator =(const Socket&) = delete;

protected:
    SOCKET m_Socket = INVALID_SOCKET;
};


class ClientSocket : public Socket
{
public:
    using Socket::Socket;

    void SetSocket(const SOCKET socket);
    bool Connect(const char* const host, const uint16_t port) const;
    bool Shutdown() const;
    bool Send(const void* const data, const size_t dataSize) const;
    bool Recv(void* const data, const size_t dataSize) const;
};


class ServerSocket : public Socket
{
public:
    using Socket::Socket;

    bool Bind(const char* const host, const uint16_t port) const;
    bool Listen() const;
    SOCKET Accept() const;
};