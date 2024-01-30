#pragma once
#include "../../core/ctx/ctx.h"
#include "../../sdk/classes/entity.h"
#include "../../hacks/features/aimbot/aimbot.hpp"
#include "../../hacks/features/radar/radar.h"
#include "../../hacks/features/triggerbot/triggerbot.h"
#include "../../hacks/features/misc/bunnyhop.hpp"

#include "../../hacks/features/misc/misc.h"

namespace cheats
{
	void radar_setting(Base_Radar& Radar);
	void run();
}

struct {
	ImFont* normal15px = nullptr;
} fonts;