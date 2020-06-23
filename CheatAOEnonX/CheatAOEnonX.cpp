#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <tchar.h>

using namespace std;
#define WindowName L"Age of Empires HD"
#define module L"empires.exe"
#define WoodStaticAddress 0x002A6858
#define Woodoffset0 0x40
#define Woodoffset1 0x4
#define Woodoffset2 0x50
#define Woodoffset3 0x4
/*
#define FoodStaticAddress 0x0001A434
#define Foodoffset 0x0
#define GoldStaticAddress 0x0001A434
#define Goldoffset 0xC
#define StoneStaticAddress 0x0001A434
#define Stoneoffset 0x8
*/

void PrintLinebyDash()
{
    int length = 10;
    for (int i = 0; i < length; ++i)
    {
        cout << "-";
    }
    cout << endl;
}

void Signature()
{
    PrintLinebyDash();
    cout << "DevKingnNNT" << endl;
    cout << "Contact Me: " << endl;
    cout << "\t" << "- Facebook: https://www.facebook.com/Kinggg.NNT" << endl;
    cout << endl;
    cout << "Have a good day!" << endl;
}

uintptr_t getModulebaseAddress(const wchar_t* modName, DWORD procId)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

int main()
{
    DWORD pid;
    HWND hGameWindow = FindWindow(NULL, WindowName);
    GetWindowThreadProcessId(hGameWindow, &pid);
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    DWORD moduleAddress = getModulebaseAddress(module, pid);
    DWORD WoodbaseAddress = 0;
    /*
    DWORD FoodbaseAddress = 0;
    DWORD GoldbaseAddress = 0;
    DWORD StonebaseAddress = 0;
    */

    if (
        !ReadProcessMemory(pHandle,
        (LPVOID)(moduleAddress + WoodStaticAddress),
            &WoodbaseAddress,
            sizeof(WoodbaseAddress),
            NULL
        )
        /*
        ||
        !ReadProcessMemory(pHandle,
        (LPVOID)(moduleAddress + FoodStaticAddress),
            &FoodbaseAddress,
            sizeof(FoodbaseAddress),
            NULL
        )
        ||
        !ReadProcessMemory(pHandle,
        (LPVOID)(moduleAddress + GoldStaticAddress),
            &GoldbaseAddress,
            sizeof(GoldbaseAddress),
            NULL
        )
        ||
        !ReadProcessMemory(pHandle,
        (LPVOID)(moduleAddress + StoneStaticAddress),
            &StonebaseAddress,
            sizeof(StonebaseAddress),
            NULL
        )
        */
        )
    {
        cout << "Couldn't read process memory";
        return -1;
    }

    DWORD WoodAddress = WoodbaseAddress + Woodoffset0 + Woodoffset1 + Woodoffset2 + Woodoffset3;
    /*
    DWORD FoodAddress = FoodbaseAddress + Foodoffset;
    DWORD GoldAddress = GoldbaseAddress + Goldoffset;
    DWORD StoneAddress = StonebaseAddress + Stoneoffset;
    */

    cout << "Hello" << endl;
    cout << "I'm KingNNT" << endl;
    cout << "You're using cheat for AOE." << endl;

    Signature();

    float newWood = 9999;
    /*
    float newFood = 9999;
    float newGold = 9999;
    float newStone = 9999;
    */

    while (true)
    {
        if (
            !WriteProcessMemory(pHandle,
            (LPVOID)(WoodAddress),
                &newWood,
                sizeof(newWood),
                0
            )
            /*
            ||
            !WriteProcessMemory(pHandle,
            (LPVOID)(FoodAddress),
                &newFood,
                sizeof(newFood),
                0
            )
            ||
            !WriteProcessMemory(pHandle,
            (LPVOID)(GoldAddress),
                &newGold,
                sizeof(newGold),
                0
            )
            ||
            !WriteProcessMemory(pHandle,
            (LPVOID)(StoneAddress),
                &newStone,
                sizeof(newStone),
                0
            )
            */
            )
        {
            cout << "Couldn't write process memory" << endl;
        }

        Sleep(1000);
    }

    return 0;
}
