#include "track.h"
#include <TlHelp32.h>
#include <iostream>
#include <vector>
#include <Windows.h>

track::track() {
	Handle = NULL;
}

track::~track() {
	CloseHandle(Handle);
}


DWORD track::GetProccID(const wchar_t* proc) {
	int ProcID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 ProcEntry;
		ProcEntry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnap, &ProcEntry)) {
			do {
				if (!_wcsicmp(ProcEntry.szExeFile, proc)) {
					ProcID = (int)ProcEntry.th32ProcessID;
					Handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcID);
				}
			} while (Process32Next(hSnap, &ProcEntry));
		}
	}
	CloseHandle(hSnap);
	return ProcID;
}

uintptr_t track::GetModuleBaseAddress(DWORD procId, const wchar_t* mod ) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modentry;
		modentry.dwSize = sizeof(modentry);
		if (Module32First(hSnap, &modentry)) {
			do {
				if (!_wcsicmp(modentry.szModule, mod)) {
					CloseHandle(hSnap);
					return (DWORD)modentry.hModule;

				}
			} while (Module32Next(hSnap, &modentry));
		}
	}

	return 0;

}

uintptr_t track::FindDMAAddy(DWORD addr, std::vector<DWORD>offsets) {
	uintptr_t ptr = addr;
	for (unsigned int i = 0; i < offsets.size(); i++) {
		ReadProcessMemory(Handle, (BYTE*)ptr, &ptr, sizeof(ptr), 0);
		ptr += offsets[i];
	}
	return ptr;
}



 
