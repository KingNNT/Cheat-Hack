// TODO: Fix Bullet
#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <tchar.h>

using namespace std;

#define WindowName L"Counter-Strike"
#define HeathModule L"hw.dll"
#define BulletModule L"client.dll"

#define HeathStaticAddress 0x00809820
#define Heathoffset 0x504
#define BulletStaticAddress 0x0011D5D8
#define Bulletoffset 0xA0

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

    DWORD HealthbaseAddress = 0;
    DWORD HealthmoduleAddress = getModulebaseAddress(HeathModule, pid);

    DWORD BulletbaseAddress = 0;
    DWORD BulletmoduleAddress = getModulebaseAddress(BulletModule, pid);

    if (
        !ReadProcessMemory(pHandle, (LPVOID)(HealthmoduleAddress + HeathStaticAddress), &HealthbaseAddress, sizeof(HealthbaseAddress), NULL) &&
        !ReadProcessMemory(pHandle, (LPVOID)(BulletmoduleAddress + BulletStaticAddress), &BulletbaseAddress, sizeof(BulletbaseAddress), NULL)
        )
    {
        cout << "Couldn't read process memory";
        return -1;
    }

    DWORD healthAddress = HealthbaseAddress + Heathoffset;
    DWORD bulletAddress = BulletbaseAddress + Bulletoffset;

    cout << "Cheat " << WindowName << " by DevKingNNT" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Health Static Address:" << (HealthmoduleAddress + HeathStaticAddress) << endl;
    cout << "Health Base Address:" << HealthbaseAddress << endl;
    cout << "Health Address: " << healthAddress << endl;
    cout << endl;
    cout << "Bullet Static Address: " << (BulletmoduleAddress + BulletStaticAddress) << endl;
    cout << "Bullet Base Address: " << (BulletbaseAddress) << endl;
    cout << "Bullet Address: " << bulletAddress << endl;
    cout << "----------------------------------------" << endl;
    cout << "Thank you! Have a good day <3";


    float newHealth = 100000;
    DWORD newBullet = 100;

    while (true)
    {
        if (
            !WriteProcessMemory(pHandle,
                                (LPVOID)(healthAddress),
                                &newHealth,
                                sizeof(newHealth),
                                0
                                )
        &&
            !WriteProcessMemory(pHandle,
                                (LPVOID)(bulletAddress),
                                &newBullet,
                                sizeof(newBullet),
                                0)
        )
        {
            cout << "Couldn't write process memory" << endl;
        }

        Sleep(1000);
    }

    return 0;
}