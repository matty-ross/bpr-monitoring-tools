#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <Windows.h>

#include "core/Logger.hpp"
#include "core/Patch.hpp"

#include "GameActions.hpp"


static constexpr char k_Name[] = "Game Actions";


GameActions GameActions::s_Instance;


namespace Patches
{
    __declspec(naked) static void HookPrintGameAction()
    {
        __asm
        {
            pushfd
            pushad

            push dword ptr [ecx - 0xC]
            push dword ptr [ecx - 0x10]
            push ecx
            mov ecx, offset GameActions::s_Instance
            call GameActions::PrintGameAction

            popad
            popfd
            
            // Original code.
            sub ecx, edi
            mov eax, 0

            push 0x07050A60
            ret
        }
    }
}


GameActions::GameActions()
    :
    m_Logger(k_Name)
{
}

GameActions& GameActions::Get()
{
    return s_Instance;
}

void GameActions::OnProcessAttach()
{
    try
    {
        Core::Logger::Initialize();

        FILE* newStdout = nullptr;
        freopen_s(&newStdout, "CONOUT$", "w", stdout);

        Core::Patch(0x07050A59, 7, m_Logger).WriteJMP(Patches::HookPrintGameAction);
    }
    catch (const std::exception& ex)
    {
        m_Logger.Error("%s", ex.what());
        MessageBoxA(NULL, ex.what(), k_Name, MB_ICONERROR);
    }
}

void GameActions::OnProcessDetach()
{
}

void GameActions::PrintGameAction(const std::byte* gameAction, int32_t gameActionID, uint32_t gameActionSize) const
{
    printf_s("%4d  [%4X] ", gameActionID, gameActionSize);
    for (uint32_t i = 0; i < gameActionSize; ++i)
    {
        printf_s(" %02X", gameAction[i]);
    }
    putchar('\n');
}
