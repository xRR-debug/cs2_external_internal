#include "globalvars.h"

bool globalvars::UpdateGlobalvars()
{
	DWORD64 m_DglobalVars = 0;
	if (!_proc_manager.read_memory<DWORD64>(g_game.get_global_vars_address(), m_DglobalVars))
		return false;

	this->address = m_DglobalVars;

	if (!this->GetRealTime())
		return false;
	if (!this->GetFrameCount())
		return false;
	if (!this->GetMaxClients())
		return false;
	if (!this->GetTickCount())
		return false;
	if (!this->GetIntervalPerTick())
		return false;
	if (!this->GetIntervalPerTick2())
		return false;
	if (!this->GetcurrentTime())
		return false;
	if (!this->GetcurrentTime2())
		return false;
	if (!this->GetCurrentNetchan())
		return false;
	if (!this->GetCurrentMap())
		return false;
	if (!this->GetCurrentMapName())
		return false;

	return true;
}

bool globalvars::GetRealTime()
{
	return get_data_address_with_offset<float>(this->address, Offset::GlobalVar.RealTime, this->g_fRealTime);
}

bool globalvars::GetFrameCount()
{
	return get_data_address_with_offset<int>(this->address, Offset::GlobalVar.FrameCount, this->g_iFrameCount);
}

bool globalvars::GetMaxClients()
{
	return get_data_address_with_offset<int>(this->address, Offset::GlobalVar.MaxClients, this->g_iMaxClients);
}

bool globalvars::GetTickCount()
{
	return get_data_address_with_offset<int>(this->address, Offset::GlobalVar.TickCount, this->g_iTickCount);
}

bool globalvars::GetIntervalPerTick()
{
	return get_data_address_with_offset<float>(this->address, Offset::GlobalVar.IntervalPerTick, this->g_fIntervalPerTick);
}

bool globalvars::GetIntervalPerTick2()
{
	return get_data_address_with_offset<float>(this->address, Offset::GlobalVar.IntervalPerTick2, this->g_fIntervalPerTick2);
}

bool globalvars::GetcurrentTime()
{
	return get_data_address_with_offset<float>(this->address, Offset::GlobalVar.CurrentTime, this->g_fCurrentTime);
}

bool globalvars::GetcurrentTime2()
{
	return get_data_address_with_offset<float>(this->address, Offset::GlobalVar.CurrentTime2, this->g_fCurrentTime2);
}

bool globalvars::GetCurrentNetchan()
{
	return get_data_address_with_offset<void*>(this->address, Offset::GlobalVar.CurrentNetchan, this->g_vCurrentNetchan);
}

bool globalvars::GetCurrentMap()
{
	return get_data_address_with_offset<char*>(this->address, Offset::GlobalVar.CurrentMap, this->g_cCurrentMap);
}

bool globalvars::GetCurrentMapName()
{
	return get_data_address_with_offset<char*>(this->address, Offset::GlobalVar.CurrentMapName, this->g_cCurrentMapName);
}