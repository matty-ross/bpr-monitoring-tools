#pragma once


#include <Windows.h>

#include "core/DetourHook.hpp"


class Direct3D
{
public:
    Direct3D();
    ~Direct3D();

private:
    void OnCreate(HRESULT hresult, const char* functionName);

private:
    static void DetourCreateBlendState();
    static void DetourCreateDepthStencilState();

private:
    DetourHook m_DetourCreateBlendState;
    DetourHook m_DetourCreateDepthStencilState;
};
