#pragma once

#include "Header.h"

DWORD GetProcId(const wchar_t* procName);
uintptr_t getModulebaseAddress(const wchar_t* modName, DWORD procId);
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
HANDLE OpenProcbyWindowName();
int CheckReadProcMemory();
void CheckWriteProcMemory();