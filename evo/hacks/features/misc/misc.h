#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <time.h>
#include <Windows.h>


#include "../../hacks/features/aimbot/aimbot.hpp"
#include "../../sdk/classes/entity.h"
#include "../../framework/cfg/cfg.hpp"
#include "../../utils/os-imgui/imgui/imgui.h"
#include "../../sdk/globals/globals.hpp"

#pragma comment(lib, "winmm.lib")

namespace misc
{
	struct Vector3
	{
		float x, y, z;
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	};

	static inline std::wstring STR2LPCWSTR(const std::string& str)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		std::wstring result(size, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size);
		return result;
	}

	static inline void cheat_text(const char* Cheat, bool config)
	{
		if (config)
			ImGui::Text(Cheat);
	}

	static inline void get_current_time(struct tm* ptm) {
		time_t now = time(NULL);
		localtime_s(ptm, &now);
	}

	void cheat_list() noexcept;
	void watermark() noexcept;
	void hit_sound(const CEntity& aLocalPlayer, int& PreviousTotalHits) noexcept;
}