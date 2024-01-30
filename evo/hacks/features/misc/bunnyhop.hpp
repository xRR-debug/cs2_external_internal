#pragma once
#include "../../sdk/classes/entity.h"
#include "../../framework/cfg/cfg.hpp"
#include "../../utils/lazy.hpp"
#include <thread>
#include <chrono>

namespace bunnyhop
{
	inline bool air_check(const CEntity& Local)
	{
	const bool has_flag_inair = Local.Pawn.HasFlag(PlayerPawn::Flags::IN_AIR);
	return has_flag_inair;
	}

	inline void run(const CEntity& Local)
	{
		if (!angel::_settings->bunnyhop)
			return;

		int force_jump;
		bool space_pressed = GetAsyncKeyState(VK_SPACE);
		bool is_inair = air_check(Local);
		g_game.get_force_jump(force_jump);

		if (space_pressed && is_inair) 
		{
			std::this_thread::sleep_for(std::chrono::microseconds(15625));
			mouse_event(MOUSEEVENTF_WHEEL, 100, 150, 200, 250);
		}
		else if (space_pressed && !is_inair)
		{
			mouse_event(MOUSEEVENTF_WHEEL, 100, 150, 200, 250);
		}
		else if (!space_pressed && force_jump == 65537)
		{
			mouse_event(MOUSEEVENTF_WHEEL, 100, 150, 200, 250);
		}
	}
}
