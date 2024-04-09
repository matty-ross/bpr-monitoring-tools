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
    static void DetourCreateRasterizerState();
    static void DetourCreateSamplerState();
    static void DetourCreateInputLayout();
    static void DetourCreateTexture2D();
    static void DetourCreateBuffer();

private:
    DetourHook m_DetourCreateBlendState;
    DetourHook m_DetourCreateDepthStencilState;
    DetourHook m_DetourCreateRasterizerState;
    DetourHook m_DetourCreateSamplerState;
    DetourHook m_DetourCreateInputLayout;
    DetourHook m_DetourCreateTexture2D;
    DetourHook m_DetourCreateBuffer;
};
