#include <exception>
#include <WinSock2.h>

#include "core/WindowsException.hpp"
#include "core/Logger.hpp"

#include "NetworkTraffic.hpp"


NetworkTraffic NetworkTraffic::s_Instance;


NetworkTraffic::NetworkTraffic()
    :
    m_Logger(k_Name),
    m_DummyServer(m_Logger)
{
}

NetworkTraffic& NetworkTraffic::Get()
{
    return s_Instance;
}

void NetworkTraffic::Load()
{
    try
    {
        Core::Logger::Initialize();

        WSADATA wsaData = {};
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0)
        {
            throw Core::WindowsException(
                HRESULT_FROM_WIN32(result),
                "Failed to initialize WinSock."
            );
        }

        m_DummyServer.Load();
    }
    catch (const std::exception& ex)
    {
        m_Logger.Error("%s", ex.what());
        MessageBoxA(NULL, ex.what(), k_Name, MB_ICONERROR);
    }
}

void NetworkTraffic::Unload()
{
    try
    {
        m_DummyServer.Unload();

        WSACleanup();
    }
    catch (const std::exception& ex)
    {
        m_Logger.Error("%s", ex.what());
        MessageBoxA(NULL, ex.what(), k_Name, MB_ICONERROR);
    }
}
