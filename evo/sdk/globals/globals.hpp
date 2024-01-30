#pragma once
#include <Windows.h>
#include "../memory/processmanager.hpp"

template <typename T>
inline bool get_data_address_with_offset(const DWORD64& Address, DWORD Offset, T& Data)
{
	if (Address == 0)
		return false;

	if (!_proc_manager.read_memory<T>(Address + Offset, Data))
		return false;

	return true;
}