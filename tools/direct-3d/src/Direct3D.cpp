#include "Direct3D.hpp"

#include <cstdio>


extern Direct3D* g_Direct3D;


Direct3D::Direct3D()
    :
    m_DetourCreateBlendState(reinterpret_cast<void*>(0x037F9760), 6, &Direct3D::DetourCreateBlendState)
{
    m_DetourCreateBlendState.Attach();
}

Direct3D::~Direct3D()
{
    m_DetourCreateBlendState.Detach();
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
        mov ecx, dword ptr[g_Direct3D]
        call Direct3D::OnCreate

        popad
        popfd
        ret
    }
}
