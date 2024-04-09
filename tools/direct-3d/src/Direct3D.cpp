#include "Direct3D.hpp"

#include <cstdio>


extern Direct3D* g_Direct3D;


Direct3D::Direct3D()
    :
    m_DetourCreateBlendState(reinterpret_cast<void*>(0x037F9760), 6, &Direct3D::DetourCreateBlendState),
    m_DetourCreateDepthStencilState(reinterpret_cast<void*>(0x0818B35C), 5, &Direct3D::DetourCreateDepthStencilState),
    m_DetourCreateRasterizerState(reinterpret_cast<void*>(0x037F9272), 6, &Direct3D::DetourCreateRasterizerState),
    m_DetourCreateSamplerState(reinterpret_cast<void*>(0x037F9E47), 6, &Direct3D::DetourCreateSamplerState),
    m_DetourCreateInputLayout(reinterpret_cast<void*>(0x081921B8), 5, &Direct3D::DetourCreateInputLayout),
    m_DetourCreateTexture2D(reinterpret_cast<void*>(0x00C8C19B), 6, &Direct3D::DetourCreateTexture2D),
    m_DetourCreateBuffer(reinterpret_cast<void*>(0x0818B08F), 6, &Direct3D::DetourCreateBuffer)
{
    m_DetourCreateBlendState.Attach();
    m_DetourCreateDepthStencilState.Attach();
    m_DetourCreateRasterizerState.Attach();
    m_DetourCreateSamplerState.Attach();
    m_DetourCreateInputLayout.Attach();
    m_DetourCreateTexture2D.Attach();
    m_DetourCreateBuffer.Attach();
}

Direct3D::~Direct3D()
{
    m_DetourCreateBlendState.Detach();
    m_DetourCreateDepthStencilState.Detach();
    m_DetourCreateRasterizerState.Detach();
    m_DetourCreateSamplerState.Detach();
    m_DetourCreateInputLayout.Detach();
    m_DetourCreateTexture2D.Detach();
    m_DetourCreateBuffer.Detach();
}

void Direct3D::OnCreate(HRESULT hresult, const char* functionName)
{
    if (FAILED(hresult))
    {
        printf_s("%s  ->  %08X\n", functionName, hresult);
    }
}

__declspec(naked) void Direct3D::DetourCreateBlendState()
{
    static constexpr char functionName[] = "ID3D11Device::CreateBlendState";

    __asm
    {
        pushfd
        pushad

        push offset functionName
        push eax
        mov ecx, dword ptr [g_Direct3D]
        call Direct3D::OnCreate

        popad
        popfd
        ret
    }
}

__declspec(naked) void Direct3D::DetourCreateDepthStencilState()
{
    static constexpr char functionName[] = "ID3D11Device::CreateDepthStencilState";

    __asm
    {
        pushfd
        pushad

        push offset functionName
        push eax
        mov ecx, dword ptr [g_Direct3D]
        call Direct3D::OnCreate

        popad
        popfd
        ret
    }
}

__declspec(naked) void Direct3D::DetourCreateRasterizerState()
{
    static constexpr char functionName[] = "ID3D11Device::CreateRasterizerState";

    __asm
    {
        pushfd
        pushad

        push offset functionName
        push eax
        mov ecx, dword ptr [g_Direct3D]
        call Direct3D::OnCreate

        popad
        popfd
        ret
    }
}

__declspec(naked) void Direct3D::DetourCreateSamplerState()
{
    static constexpr char functionName[] = "ID3D11Device::CreateSamplerState";

    __asm
    {
        pushfd
        pushad

        push offset functionName
        push eax
        mov ecx, dword ptr [g_Direct3D]
        call Direct3D::OnCreate

        popad
        popfd
        ret
    }
}

__declspec(naked) void Direct3D::DetourCreateInputLayout()
{
    static constexpr char functionName[] = "ID3D11Device::CreateInputLayout";

    __asm
    {
        pushfd
        pushad

        push offset functionName
        push eax
        mov ecx, dword ptr [g_Direct3D]
        call Direct3D::OnCreate

        popad
        popfd
        ret
    }
}

__declspec(naked) void Direct3D::DetourCreateTexture2D()
{
    static constexpr char functionName[] = "ID3D11Device::CreateTexture2D";

    __asm
    {
        pushfd
        pushad

        push offset functionName
        push eax
        mov ecx, dword ptr [g_Direct3D]
        call Direct3D::OnCreate

        popad
        popfd
        ret
    }
}

__declspec(naked) void Direct3D::DetourCreateBuffer()
{
    static constexpr char functionName[] = "ID3D11Device::CreateBuffer";

    __asm
    {
        pushfd
        pushad

        push offset functionName
        push eax
        mov ecx, dword ptr [g_Direct3D]
        call Direct3D::OnCreate

        popad
        popfd
        ret
    }
}
