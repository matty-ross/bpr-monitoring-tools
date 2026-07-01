#include <exception>
#include <Windows.h>

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
    }
    catch (const std::exception& ex)
    {
        m_Logger.Error("%s", ex.what());
        MessageBoxA(NULL, ex.what(), k_Name, MB_ICONERROR);
    }
}
