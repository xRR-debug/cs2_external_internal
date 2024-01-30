#include "configmenu.hpp"

namespace ConfigMenu 
{
	void render_config_menu(const char *Tab) 
	{	
			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnOffset(1, 250.0f);

			ImGui::PushItemWidth(250.0f);

			static char configNameBuffer[128] = "default";

			static int selectedConfig = -1;

			const std::string configDir = path::path;
			static std::vector<std::string> configFiles;
			std::vector<const char*> configFilesCStr;

			configFiles.clear();
			for (const auto& entry : std::filesystem::directory_iterator(configDir))
			{
				if (entry.is_regular_file() && entry.path().extension() == "")
				{
					configFiles.push_back(entry.path().filename().string());
				}
			}

			for (const auto& file : configFiles)
			{
				configFilesCStr.push_back(file.c_str());
			}

			ImGui::ListBox(X("##ConfigFiles"), &selectedConfig, configFilesCStr.data(), configFilesCStr.size());


			if (ImGui::Button(X("Load"), { 70.0f, 20.0f }) && selectedConfig >= 0 && selectedConfig < configFiles.size())
			{
				std::string selectedConfigFile = configFiles[selectedConfig];
				angel::_settings->load(selectedConfigFile);
			}

			ImGui::SameLine();

			if (ImGui::Button(X("Save"), { 70.0f, 20.0f }) && selectedConfig >= 0 && selectedConfig < configFiles.size())
			{
				std::string selectedConfigFile = configFiles[selectedConfig];
				angel::_settings->save(selectedConfigFile);
			}

			if (ImGui::Button(X("Delete"), { 70.0f, 20.0f }) && selectedConfig >= 0 && selectedConfig < configFiles.size())
				ImGui::OpenPopup(X("##reallyDelete"));

			if (ImGui::BeginPopup(X("##reallyDelete")))
			{
				ImGui::TextUnformatted(X("    Are you sure?"));
				if (ImGui::Button(X("No"), { 45.0f, 0.0f }))
					ImGui::CloseCurrentPopup();
				ImGui::SameLine();
				if (ImGui::Button(X("Yes"), { 45.0f, 0.0f }))
				{
					std::string selectedConfigFile = configFiles[selectedConfig];
					std::string fullPath = configDir + "\\" + selectedConfigFile;
					if (std::remove(fullPath.c_str()) == 0)
					{
						configFiles.erase(configFiles.begin() + selectedConfig);
						selectedConfig = -1;
					}
					else
					{
					}
					ImGui::CloseCurrentPopup();
				}
			ImGui::EndPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button(X("Reset"), { 70.0f, 20.0f }))
				ImGui::OpenPopup(X("##reallyReset"));
			if (ImGui::BeginPopup(X("##reallyReset")))
			{
				ImGui::TextUnformatted(X("    Are you sure?"));
				if (ImGui::Button(X("No"), { 45.0f, 0.0f }))
					ImGui::CloseCurrentPopup();
				ImGui::SameLine();
				if (ImGui::Button(X("Yes"), { 45.0f, 0.0f }))
				{
					ConfigMenu::reset_to_default();
					ImGui::CloseCurrentPopup();
				}	
				ImGui::EndPopup();
			}
			ImGui::Columns(0);

			ImGui::InputText(" ", configNameBuffer, sizeof(configNameBuffer));

			if (ImGui::Button(X("Create Config"), { 80.0f, 20.0f }))
			{
				std::string configFileName = std::string(configNameBuffer) + "";
				angel::_settings->save(configFileName);
			}
			ImGui::SameLine();

			if (ImGui::Button(X("Config Folder"), { 80.0f, 20.0f }))
			{
				Gui.OpenWebpage(configDir.c_str());
			}


	}

	void reset_to_default() 
	{
		angel::_settings->ammobar = false;
		angel::_settings->outLine = true;
		angel::_settings->filled_color = ImColor(255, 255, 255, 255);
		angel::_settings->filled_color2 = ImColor(255, 255, 255, 255);
		angel::_settings->multi_color = false;
		angel::_settings->filled_vis_color = ImColor(0, 0, 255, 255);
		angel::_settings->filled_vis_box = false;
		angel::_settings->visible_color = ImColor(0, 0, 0, 255);
		angel::_settings->esp_visible_check = true;
		angel::_settings->box_alpha = 0.0;
		angel::_settings->show_distance = false;
		angel::_settings->bone_esp = false;
		angel::_settings->box_esp = true;
		angel::_settings->healthbar = true;
		angel::_settings->weapon_esp = false;
		angel::_settings->player_name = true;
		angel::_settings->box_rounding = 0.0;
		angel::_settings->esp_enabled = false;
		angel::_settings->draw_fov = false;
		angel::_settings->bone_color = ImColor(0, 0, 0, 255);
		angel::_settings->box_color = ImColor(0, 255, 0, 255);
		angel::_settings->show_radar = false;
		angel::_settings->radar_range = 150;
		angel::_settings->show_radar_crossline = false;
		angel::_settings->radar_crossline_color = ImColor(220, 220, 220, 255);
		angel::_settings->radar_type = 2;
		angel::_settings->radar_point_size_proportion = 1;
		angel::_settings->radar_bg_alpha = 0.1;
		angel::_settings->proportion = 3300;
		triggerbot::trigger_delay = 0;
		triggerbot::fake_shot_delay = 10;
		triggerbot::set_hotkey(angel::_settings->triggerbot_hotkey);
		aimbot_t::set_hotkey(angel::_settings->aimbot_hotkey);
		aimbot_t::RCSScale = ImVec2(1.2, 1.4);
		rcs::rcs_bullet = 1;
		angel::_settings->bunnyhop = false;
		angel::_settings->work_in_spec = true;
		angel::_settings->watermark = false;
		angel::_settings->cheat_list = false;
		angel::_settings->hitsound = false;
		angel::_settings->bomb_timer_color = ImColor(255, 120, 0, 255);
		angel::_settings->bomb_timer = false;
		angel::_settings->show_menu = true;
		angel::_settings->aimbot = false;
		angel::_settings->aimbot_always = false;
		angel::_settings->aimbot_toggle_mode = false;
		angel::_settings->aim_position = 0;
		angel::_settings->aim_hitbox_position = BONEINDEX::head;
		angel::_settings->box_type = 0;
		angel::_settings->healthbar_type = 0;
		angel::_settings->triggerbot = false;
		angel::_settings->triggerbot_always = false;
		angel::_settings->team_check = true;
		angel::_settings->obsbypass = true;
		angel::_settings->rcsfov = 5;
		angel::_settings->aimbot_visible_check = true;
		angel::_settings->crosshair_placement = false;
		angel::_settings->crosshair_placement_color = ImColor(255, 255, 255, 200);
		angel::_settings->aimbot_hotkey = 0;
		angel::_settings->triggerbot_hotkey = 0;
		angel::_settings->fov_circle_color = ImColor(255, 255, 255, 255);
	}
}
