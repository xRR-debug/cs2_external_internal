#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include <Tlhelp32.h>
#include <atlconv.h>
#include "../../sdk/memory/handlehijack.h"
#include "../../utils/lazy.hpp"

#define _is_invalid(v) if(v==NULL) return false
#define _is_invalid(v,n) if(v==NULL) return n

typedef NTSTATUS(WINAPI* pNtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesRead);
typedef NTSTATUS(WINAPI* pNtWriteVirtualMemory)(HANDLE Processhandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);

enum StatusCode
{
	SUCCEED,
	FAILE_PROCESSID,
	FAILE_HPROCESS,
	FAILE_MODULE,
};

class ProcessManager 
{
private:
	bool   attached = false;
	pNtReadVirtualMemory VRead; // define Virtual Read + Virtual Write
	pNtWriteVirtualMemory VWrite;


public:

	HANDLE hProcess = 0;
	DWORD  ProcessID = 0;
	DWORD64  ModuleAddress = 0;




public:
	~ProcessManager() 
	{

		VRead = (pNtReadVirtualMemory)GetProcAddress(GetModuleHandleA(X("ntdll.dll")), X("NtReadVirtualMemory"));
		VWrite = (pNtWriteVirtualMemory)GetProcAddress(GetModuleHandleA(X("ntdll.dll")), X("NtWriteVirtualMemory"));
	}

	void hide_from_debuger()
	{
		typedef NTSTATUS(NTAPI* pfnNtSetInformationThread)(
			_In_ HANDLE ThreadHandle,
			_In_ ULONG  ThreadInformationClass,
			_In_ PVOID  ThreadInformation,
			_In_ ULONG  ThreadInformationLength
			);
		const ULONG ThreadHideFromDebugger = 0x11;

		if (auto lla = LI_FN(LoadLibraryA).forwarded_safe_cached()) {
			if (auto hNtDll = lla(X("ntdll.dll"))) {

				if (auto gpa = LI_FN(GetProcAddress).forwarded_safe_cached()) {
					pfnNtSetInformationThread NtSetInformationThread = (pfnNtSetInformationThread)
						gpa(hNtDll, X("NtSetInformationThread"));
					if (auto gct = LI_FN(GetCurrentThread).forwarded_safe_cached()) {
						NTSTATUS status = NtSetInformationThread(gct(),
							ThreadHideFromDebugger, NULL, 0);
					}
				}
			}
		}
	}

	StatusCode attach(std::string ProcessName)
	{
		ProcessID = this->get_process_id(ProcessName);
		_is_invalid(ProcessID, FAILE_PROCESSID);

		hProcess = hijack::hijack_existing_handle(ProcessID);
		//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
		
		_is_invalid(hProcess, FAILE_HPROCESS);

		ModuleAddress = reinterpret_cast<DWORD64>(this->get_process_module_handle(ProcessName));
		_is_invalid(ModuleAddress, FAILE_MODULE);

		attached = true;

		return SUCCEED;

		hide_from_debuger();
		
	}

	void detach()
	{
		if (hProcess)
			LI_FN(CloseHandle).forwarded_safe_cached()(hProcess);
		hProcess = 0;
		ProcessID = 0;
		ModuleAddress = 0;
		attached = false;
	}

	bool is_active()
	{
		if (!attached)
			return false;
		DWORD ExitCode{};
		GetExitCodeProcess(hProcess, &ExitCode);
		return ExitCode == STILL_ACTIVE;
	}

	template <typename ReadType>
	constexpr const ReadType read_ptr(const std::uintptr_t& address) const noexcept
	{
		ReadType value = { };
		ReadProcessMemory(hProcess, reinterpret_cast<const void*>(address), &value, sizeof(ReadType), NULL);
		return value;
	}

	template <typename ReadType>
	bool read_memory(DWORD64 Address, ReadType& Value, int Size)
	{
		_is_invalid(hProcess,false);
		_is_invalid(ProcessID, false);

		if (ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address), &Value, Size, 0))
			return true;
		return false;
	}

	template <typename ReadType>
	bool read_memory(DWORD64 Address, ReadType& Value)
	{
		_is_invalid(hProcess, false);
		_is_invalid(ProcessID, false);

		if (ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address), &Value, sizeof(ReadType), 0))
			return true;
		return false;
	}

	template <typename ReadType>
	bool write_memory(DWORD64 Address, ReadType& Value, int Size)
	{
		_is_invalid(hProcess, false);
		_is_invalid(ProcessID, false);

		if (WriteProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address), &Value, Size, 0))
			return true;
		return false;
	}

	template <typename ReadType>
	bool write_memory(DWORD64 Address, ReadType& Value)
	{
		_is_invalid(hProcess, false);
		_is_invalid(ProcessID, false);

		if (WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(Address), &Value, sizeof(ReadType), 0))
			return true;
		return false;
	}

	template <typename ReadType> 
	bool read_virtual_memory(DWORD64 address)
	{
		ReadType buffer{ };
		VRead(hProcess, (void*)address, &buffer, sizeof(ReadType), 0);
		return buffer;
	}

	template <typename ReadType>
	bool write_vitrual_memory(DWORD64 address, ReadType value)
	{
		VWrite(hProcess, (void*)address, &value, sizeof(ReadType), NULL);
		return value;
	}

	const bool read_raw(uintptr_t address, void* buffer, size_t size)
	{
		SIZE_T bytesRead;
		if (ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(address), buffer, size, &bytesRead))
		{
			return bytesRead == size;
		}
		return false;
	}

	const std::string read_string(std::uint64_t dst)
	{
		if (!dst)
			return X("**invalid**");

		char buf[256] = {};
		return (read_raw(dst, &buf, sizeof(buf)) ? std::string(buf) : X("**invalid**"));
	}

	std::vector<DWORD64> search_memory(const std::string& Signature, DWORD64 StartAddress, DWORD64 EndAddress, int SearchNum = 1);

	DWORD64 trace_address(DWORD64 BaseAddress, std::vector<DWORD> Offsets)
	{
		_is_invalid(hProcess,0);
		_is_invalid(ProcessID,0);
		DWORD64 Address = 0;

		if (Offsets.size() == 0)
			return BaseAddress;

		if (!read_memory<DWORD64>(BaseAddress, Address))
			return 0;
	
		for (int i = 0; i < Offsets.size() - 1; i++)
		{
			if (!read_memory<DWORD64>(Address + Offsets[i], Address))
				return 0;
		}
		return Address == 0 ? 0 : Address + Offsets[Offsets.size() - 1];
	}

public:

	DWORD get_process_id(std::string ProcessName)
	{
		PROCESSENTRY32 ProcessInfoPE;
		ProcessInfoPE.dwSize = sizeof(PROCESSENTRY32);
		HANDLE hSnapshot = CreateToolhelp32Snapshot(15, 0);
		Process32First(hSnapshot, &ProcessInfoPE);
		USES_CONVERSION;
		do {
			if (strcmp(W2A(ProcessInfoPE.szExeFile), ProcessName.c_str()) == 0)
			{
				CloseHandle(hSnapshot);
				return ProcessInfoPE.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &ProcessInfoPE));
		CloseHandle(hSnapshot);
		return 0;
	}

	HMODULE get_process_module_handle(std::string ModuleName)
	{
		MODULEENTRY32 ModuleInfoPE;
		ModuleInfoPE.dwSize = sizeof(MODULEENTRY32);
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->ProcessID);
		Module32First(hSnapshot, &ModuleInfoPE);
		USES_CONVERSION;
		do {
			if (strcmp(W2A(ModuleInfoPE.szModule), ModuleName.c_str()) == 0)
			{
				CloseHandle(hSnapshot);
				return ModuleInfoPE.hModule;
			}
		} while (Module32Next(hSnapshot, &ModuleInfoPE));
		CloseHandle(hSnapshot);
		return 0;
	}

	template <typename T = std::uintptr_t>
	T* get_absolute_address(T* pRelativeAddress, int nPreOffset = 0x0, int nPostOffset = 0x0)
	{
		pRelativeAddress += nPreOffset;
		pRelativeAddress += sizeof(std::int32_t) + *reinterpret_cast<std::int32_t*>(pRelativeAddress);
		pRelativeAddress += nPostOffset;
		return pRelativeAddress;
	}

	std::uintptr_t resolve_relative_address(std::uintptr_t nAddressBytes, std::uint32_t nRVAOffset, std::uint32_t nRIPOffset, std::uint32_t nOffset = 0) {
		const std::uintptr_t nRVA = *reinterpret_cast<PLONG>(nAddressBytes + nRVAOffset);
		const std::uintptr_t nRIP = nAddressBytes + nRIPOffset;

		if (nOffset)
			return read_ptr< std::uintptr_t >(nRVA + nRIP) + nOffset;

		return nRVA + nRIP;
	}

};

inline ProcessManager _proc_manager;