#pragma once
#include <iostream>
#include <windows.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <shlobj_core.h>

#include "../../sdk/classes/bone.h"
#include "../../core/ctx/ctx.h"
#include "../../utils/random.h"

namespace path
{
	inline std::string path = "";
}

namespace angel {
	class settings_t {
	public:
		bool save(std::string file_name);
		bool load(std::string file_name);
		//main

		bool aimbot = false;
		bool aimbot_always = false;
		bool aimbot_toggle_mode = false;
		int aimbot_hotkey = 0;
		int  aim_position = 0;
		DWORD  aim_hitbox_position = BONEINDEX::head;
		bool aimbot_visible_check = true;
		bool aimbot_curve = false;
		bool aimbot_scopecheck = true;

		bool rcs = false;

		bool weapon_config = false;

		float rifle_fov = 0;
		float rifle_smooth = 0;

		float pistol_fov = 0;
		float pistol_smooth = 0;

		float sniper_fov = 0;
		float sniper_smooth = 0;

		float heavysmg_fov = 0;
		float heavysmg_smooth = 0;

		int box_type = 0;
		int  healthbar_type = 0;
		float aimbotfov = 0;
		float rcsfov = 0;

		ImColor fov_circle_color = ImColor(255, 255, 255, 255);

		bool show_menu = true;

		bool triggerbot = false;
		bool triggerbot_always = false;
		int triggerbot_hotkey = 0;

		bool team_check = true;

		bool crosshair_placement = false;
		ImColor crosshair_placement_color = ImColor(255, 255, 255, 200);

		bool obsbypass = true;
		//esp
		 bool esp_enabled = true;
		 bool ammobar = false;
		 bool bone_esp = false;
		 bool box_esp = true;
		 bool healthbar = true;
		 bool weapon_esp = true;
		 bool scoped_esp = true;
		 bool player_name = true;
		 bool draw_fov = false;
		 bool show_distance = false;
		 bool esp_visible_check = true;
		 bool filled_box = false;
		 bool filled_vis_box = false;
		 bool multi_color = true;
		 bool outLine = false;
		 float box_rounding = 3.0;
		 float box_alpha = 0.35;

		 ImColor bone_color = ImColor(0, 0, 0, 255);
		 ImColor box_color = ImColor(0, 255, 0, 255);
		 ImColor visible_color = ImColor(0, 0, 0, 255);
		 ImColor filled_color = ImColor(255, 255, 255, 128);
		 ImColor filled_vis_color = ImColor(0, 167, 255, 255);
		 ImColor filled_color2 = ImColor(167, 50, 255, 255);
		//crosshair
		 bool crosshair_team_check = true;
		 bool is_aim = false;
		 bool is_jump = false;
		//radar
		bool show_radar = false;
		float radar_range = 150;
		float radar_point_size_proportion = 1;
		bool show_radar_crossline = false;
		ImColor radar_crossline_color = ImColor(255, 255, 255, 255);
		int radar_type = 2;
		float proportion = 3300;
		bool custom_radar = false;
		float radar_bg_alpha = 0.1 ;
		//misc
		bool bunnyhop = false;
		bool work_in_spec = true;
		bool watermark = false;
		bool cheat_list = false;
		bool hitsound = false;
		int hitsound_type = 0;
		bool bomb_timer = false;
		bool discordrpc = true;
		bool unload = false;
		ImColor bomb_timer_color = ImColor(255, 120, 0, 255);
	};

	inline const auto _settings = std::make_unique< settings_t >();
}
