#include "Header.h"
using namespace std;

DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
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

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

HANDLE OpenProcbyWindowName()
{
	HWND hGameWindow = FindWindow(NULL, WindowName);
	GetWindowThreadProcessId(hGameWindow, &pid);
	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	return pHandle;
}

int CheckReadProcMemory()
{
	HANDLE pHandle = OpenProcbyWindowName();
	if (
		!ReadProcessMemory(pHandle, (LPVOID)(moduleAddress + WoodStaticAddress), &WoodbaseAddress, sizeof(WoodbaseAddress), NULL)
		||
		!ReadProcessMemory(pHandle, (LPVOID)(moduleAddress + FoodStaticAddress), &FoodbaseAddress, sizeof(FoodbaseAddress), NULL)
		||
		!ReadProcessMemory(pHandle, (LPVOID)(moduleAddress + GoldStaticAddress), &GoldbaseAddress, sizeof(GoldbaseAddress), NULL)
		||
		!ReadProcessMemory(pHandle, (LPVOID)(moduleAddress + StoneStaticAddress), &StonebaseAddress, sizeof(StonebaseAddress), NULL)
		)
	{
		cout << "Couldn't read process memory";
		return -1;
	}
}

void CheckWriteProcMemory()
{
	HANDLE pHandle = OpenProcbyWindowName();
	int delay = 1000; // Unit: Milisecond
	while (true)
	{
		if (
			!WriteProcessMemory(pHandle, (LPVOID)(WoodAddress), &newWood, sizeof(newWood), 0)
			||
			!WriteProcessMemory(pHandle, (LPVOID)(FoodAddress), &newFood, sizeof(newFood), 0)
			||
			!WriteProcessMemory(pHandle, (LPVOID)(GoldAddress), &newGold, sizeof(newGold), 0)
			||
			!WriteProcessMemory(pHandle, (LPVOID)(StoneAddress), &newStone, sizeof(newStone), 0)
			)
		{
			cout << "Couldn't write process memory" << endl;
		}

		Sleep(delay);
	}
}