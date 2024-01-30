#pragma once
#include <optional>
#include "../../sdk/memory/processmanager.hpp"
#include "../../sdk/offsets/offsets.h"
#include "../../sdk/classes/view.hpp"

class CGame
{
private:
	struct
	{
		DWORD64 serverdll;
		DWORD64 clientdll;
		DWORD64 entitylist;
		DWORD64 matrix;
		DWORD64 viewangle;
		DWORD64 entitylistentry;
		DWORD64 localcontroller;
		DWORD64 localpawn;
		DWORD64 serverpawn;
		DWORD64 forcejump;
		DWORD64 globalvars;
	}Address;

public:
	CView view;

public:	

	bool init_address();

	DWORD64 get_client_dll_address();
	DWORD64 get_server_dll_address();

	DWORD64 get_entity_list_address();

	DWORD64 get_matrix_address();

	DWORD64 get_view_angle_address();

	DWORD64 get_entity_list_entry();

	DWORD64 get_local_controller_address();

	DWORD64 get_local_pawn_address();

	DWORD64 get_server_pawn_address();

	DWORD64 get_global_vars_address();

	bool update_entity_list_entry();

	bool set_view_angle(float Yaw, float Pitch);

	bool set_force_jump(int Value);
	bool get_force_jump(int& Value);
};

inline CGame g_game;