#include "offsets.h"

DWORD64 search_offsets(std::string Signature, DWORD64 ModuleAddress)
{
	std::vector<DWORD64> TempAddressList;
	DWORD64 Address = 0;
	DWORD Offsets = 0;

	TempAddressList = _proc_manager.search_memory(Signature, ModuleAddress, ModuleAddress + 0x4000000);

	if (TempAddressList.size() <= 0)
		return 0;

	if (!_proc_manager.read_memory<DWORD>(TempAddressList.at(0) + 3, Offsets))
		return 0;

	Address = TempAddressList.at(0) + Offsets + 7;
	return Address;
}

bool Offset::update_offsets()
{
	DWORD64 ClientDLL = reinterpret_cast<DWORD64>(_proc_manager.get_process_module_handle("client.dll"));
	if (ClientDLL == 0)
		return false;

	DWORD64 ServerDLL = reinterpret_cast<DWORD64>(_proc_manager.get_process_module_handle("server.dll"));
	if (ServerDLL == 0)
		return false;

	DWORD64 InputDLL = reinterpret_cast<DWORD64>(_proc_manager.get_process_module_handle("inputsystem.dll"));
	if (InputDLL == 0)
		return false;

	DWORD64 TempAddress = 0;

	TempAddress = search_offsets(Offset::Signatures::EntityList, ClientDLL);
	if (TempAddress == 0)
		return false;

	Offset::EntityList = TempAddress - ClientDLL;

	TempAddress = search_offsets(Offset::Signatures::LocalPlayerController, ClientDLL);
	if (TempAddress == 0)
		return false;

	Offset::LocalPlayerController = TempAddress - ClientDLL;

	TempAddress = search_offsets(Offset::Signatures::ViewMatrix, ClientDLL);
	if (TempAddress == 0)
		return false;

	Offset::Matrix = TempAddress - ClientDLL;

	TempAddress = search_offsets(Offset::Signatures::GlobalVars, ClientDLL);
	if (TempAddress == 0)
		return false;

	Offset::GlobalVars = TempAddress - ClientDLL;

	TempAddress = search_offsets(Offset::Signatures::ViewAngles, ClientDLL);
	if (TempAddress == 0)
		return false;
	if (!_proc_manager.read_memory(TempAddress, TempAddress))
		return false;

	Offset::ViewAngle = TempAddress + 0x8F30 - ClientDLL;

	TempAddress = search_offsets(Offset::Signatures::LocalPlayerPawn, ClientDLL);
	if (TempAddress == 0)
		return false;

	Offset::LocalPlayerPawn = TempAddress + 0x138 - ClientDLL;

	TempAddress = search_offsets(Offset::Signatures::ForceJump, ClientDLL);
	if (TempAddress == 0)
		return false;

	Offset::ForceJump = TempAddress + 0x30 - ClientDLL;

	TempAddress = search_offsets(Offset::Signatures::PlantedC4, ClientDLL);
	if (TempAddress == 0)
		return false;

	Offset::PlantedC4 = TempAddress - ClientDLL;

	TempAddress = search_offsets(Offset::Signatures::InputSystem, InputDLL);
	if (TempAddress == 0)
		return false;

	Offset::InputSystem = TempAddress - InputDLL;
	return true;
}