#pragma once
#include <math.h>
#include "../../sdk/classes/entity.h"
#include "../../sdk/classes/view.hpp"
#include "../../framework/cfg/cfg.hpp"

namespace rcs
{
	inline int rcs_bullet = 1;

	inline void update_angles(const CEntity& local, Vec2& angles)
	{
		auto old_punch = Vec2{ };
		auto shots_fired = local.Pawn.ShotsFired;

		int screen_center_x = Gui.Window.Size.x / 2;
		int screen_center_y = Gui.Window.Size.y / 2;

		if (shots_fired)
		{
			uintptr_t clientState;
			auto viewangles = local.Pawn.ViewAngle;
			auto aimpunch = local.Pawn.AimPunchAngle;

			auto newangles = Vec2
			{
				viewangles.x + old_punch.x - aimpunch.x * 2.f,
				viewangles.y + old_punch.y - aimpunch.y * 2.f,
			};

			if (newangles.x > 89.f)
				newangles.x = 89.f;

			if (newangles.x < -89.f)
				newangles.x = -89.f;

			while (newangles.y > 180.f)
				newangles.y -= 360.f;

			while (newangles.y < -180.f)
				newangles.y += 360.f;

			newangles.x += screen_center_x;
			newangles.y += screen_center_y;
			angles = newangles;
			old_punch = aimpunch;
		}
		else
		{
			old_punch.x = old_punch.y = 0.f;
		}

		if (local.Pawn.ShotsFired > rcs_bullet)
		{
			Vec2 punch_angle;
			if (local.Pawn.AimPunchCache.Count <= 0 && local.Pawn.AimPunchCache.Count > 0xFFFF)
				return;
			if (!_proc_manager.read_memory<Vec2>(local.Pawn.AimPunchCache.Data + (local.Pawn.AimPunchCache.Count - 1) * sizeof(Vec3), punch_angle))
				return;

			angles.x = punch_angle.x;
			angles.y = punch_angle.y;
		}
	}
}
