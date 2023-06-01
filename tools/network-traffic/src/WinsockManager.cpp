#include "WinsockManager.h"

#include <Windows.h>
#include <WinSock2.h>


WinsockManager::WinsockManager()
{
    constexpr WORD versionRequired = MAKEWORD(2, 2);

    WSADATA wsaData = {};
    if (WSAStartup(versionRequired, &wsaData) != 0)
    {
        MessageBoxA(nullptr, "Failed to initialize Winsock library!", "ERROR", MB_ICONERROR);
    }
}

WinsockManager::~WinsockManager()
{
    WSACleanup();
}