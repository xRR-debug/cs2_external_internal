#include "ctx.h" //game

bool CGame::init_address()
{
	this->Address.clientdll = reinterpret_cast<DWORD64>(_proc_manager.get_process_module_handle("client.dll"));
	this->Address.serverdll = reinterpret_cast<DWORD64>(_proc_manager.get_process_module_handle("server.dll"));
	
	this->Address.entitylist = get_client_dll_address() + Offset::EntityList;
	this->Address.matrix = get_client_dll_address() + Offset::Matrix;
	this->Address.viewangle = get_client_dll_address() + Offset::ViewAngle;
	this->Address.localcontroller = get_client_dll_address() + Offset::LocalPlayerController;
	this->Address.localpawn = get_client_dll_address() + Offset::LocalPlayerPawn;
	this->Address.serverpawn = get_server_dll_address() + Offset::LocalPlayerPawn;
	this->Address.forcejump = get_client_dll_address() + Offset::ForceJump;
	this->Address.globalvars = get_client_dll_address() + Offset::GlobalVars;

	return this->Address.clientdll != 0;
}

DWORD64 CGame::get_client_dll_address()
{
	return this->Address.clientdll;
}

DWORD64 CGame::get_server_dll_address()
{
	return this->Address.serverdll;
}

DWORD64 CGame::get_entity_list_address()
{
	return this->Address.entitylist;
}

DWORD64 CGame::get_matrix_address()
{
	return this->Address.matrix;
}

DWORD64 CGame::get_view_angle_address() 
{
	return this->Address.viewangle;
}

DWORD64 CGame::get_entity_list_entry()
{
	return this->Address.entitylistentry;
}

DWORD64 CGame::get_local_controller_address()
{
	return this->Address.localcontroller;
}

DWORD64 CGame::get_local_pawn_address()
{
	return this->Address.localpawn;
}

DWORD64 CGame::get_server_pawn_address()
{
	return this->Address.serverpawn;
}

DWORD64 CGame::get_global_vars_address()
{
	return this->Address.globalvars;
}


bool CGame::update_entity_list_entry()
{
	DWORD64 EntityListEntry = 0;
	if (!_proc_manager.read_memory<DWORD64>(g_game.get_entity_list_address(), EntityListEntry))
		return false;
	if (!_proc_manager.read_memory<DWORD64>(EntityListEntry + 0x10, EntityListEntry))
		return false;

	this->Address.entitylistentry = EntityListEntry;

	return this->Address.entitylistentry != 0;
}

bool CGame::set_view_angle(float Yaw, float Pitch)
{
	Vec2 Angle{ Pitch,Yaw };

	if (!_proc_manager.write_memory<Vec2>(this->Address.viewangle, Angle))
		return false;

	return true;
}

bool CGame::set_force_jump(int value)
{
	if (!_proc_manager.write_memory<int>(this->Address.forcejump, value))
		return false;

	return true;
}
bool CGame::get_force_jump(int& value)
{
	if (!_proc_manager.read_memory<int>(this->Address.forcejump, value))
		return false;

	return true;
}