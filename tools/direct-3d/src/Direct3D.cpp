#include "Direct3D.hpp"

#include <cstdio>


extern Direct3D* g_Direct3D;


Direct3D::Direct3D()
    :
    m_DetourCreateBlendState(reinterpret_cast<void*>(0x037F9760), 6, &Direct3D::DetourCreateBlendState),
    m_DetourCreateDepthStencilState(reinterpret_cast<void*>(0x0818B35C), 5, &Direct3D::DetourCreateDepthStencilState)
{
    m_DetourCreateBlendState.Attach();
    m_DetourCreateDepthStencilState.Attach();
}

Direct3D::~Direct3D()
{
    m_DetourCreateBlendState.Detach();
    m_DetourCreateDepthStencilState.Detach();
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
