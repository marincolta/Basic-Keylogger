#pragma once
#include<vector>
#include<Windows.h>
#include<TlHelp32.h>

class track
{
public:
	track();
	~track();



	DWORD GetProccID(const wchar_t*);
	uintptr_t GetModuleBaseAddress(DWORD, const wchar_t*);
	uintptr_t FindDMAAddy(DWORD addr, std::vector<DWORD>);

	HANDLE Handle;
};



