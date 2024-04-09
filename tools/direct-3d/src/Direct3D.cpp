#include "Direct3D.hpp"

#include <cstdio>


extern Direct3D* g_Direct3D;


Direct3D::Direct3D()
{
}

Direct3D::~Direct3D()
{
}

void Direct3D::OnCreate(HRESULT hresult, const char* functionName)
{
    if (FAILED(hresult))
    {
        printf_s("%s  ->  %08X\n", functionName, hresult);
    }
}
