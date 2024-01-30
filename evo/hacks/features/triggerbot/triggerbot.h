#pragma once
#include <chrono>
#include <thread>

#include "../../core/ctx/ctx.h"
#include "../../framework/cfg/cfg.hpp"
#include "../../sdk/classes/entity.h"

namespace triggerbot
{
	inline int trigger_delay = 5;
	inline int fake_shot_delay = 20; 
	inline int hotkey = VK_LMENU;
	inline std::vector<int> hotkey_list{VK_LMENU, VK_RBUTTON, VK_XBUTTON1, VK_XBUTTON2, VK_CAPITAL, VK_LSHIFT, VK_LCONTROL};

	inline void set_hotkey(int index)
	{
		hotkey = hotkey_list.at(index);
	}

	void release_mouse_event();
	void run(const CEntity& LocalEntity);
	void target_check(const CEntity& LocalEntity) noexcept;
}