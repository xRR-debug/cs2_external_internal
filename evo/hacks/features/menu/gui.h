#pragma once
#include "../../framework/cfg/cfg.hpp"
#include "../../sdk/render/render.hpp"
#include "../../hacks/features/aimbot/aimbot.hpp"
#include "../../hacks/features/radar/radar.h"
#include "../../hacks/features/triggerbot/triggerbot.h"
#include "../../framework/cfg/configmenu.hpp"
#include "../../hacks/features/menu/font/iconsfontwwesome5.h"
#include "../../utils/os-imgui/custom.hpp"
#include "../../inc.hpp"

namespace gui
{
	void main_menu()
	{
		{
			using namespace ImGui;

			static bool bools[50]{};
			static int ints[50]{};

			vector < const char* > animation_types = { X("left to right"),
												X("middle pulse"),
												X("tiny color") };

			vector < const char* > keymode = { X("hold"),
											X("toggle"),
											X("always") };

			vector < const char* > hitsound = {
							   X("neverlose"),
							   X("skeet"),
							   X("primordial"),
							   X("break"),
							   X("cookie"),
							   X("moan"),
							   X("rifk"),
							   X("roblox"),
							   X("button"),
							   X("bubble"),
							   X("shine")
			};

			vector < const char* > hitboxes = { "head",
										"neck",
										"chest", "lower chest", "pelvis","nearest"};

			vector < const char* > aimbot_key_binds = { "mouse1","alt", "mouse2", "mouse3", "mouse5", "capslock", "shift", "ctrl", };
			vector < const char* > triggerbot_key_binds = { "alt", "mouse2", "mouse3", "mouse5", "capslock", "shift", "ctrl",};

			static float color[4] = { 1.f, 1.f, 1.f, 1.f };

			PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(510, 500));

			char TempText[256];
			ImGui::Begin(X("angeldancy"), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
			{
				auto window = GetCurrentWindow();
				auto draw = window->DrawList;
				auto pos = window->Pos;
				auto size = window->Size;
				auto style = GetStyle();

				custom.m_anim = ImLerp(custom.m_anim, 1.f, 0.1f);

				draw->AddLine(pos + ImVec2(size.y - 1000, 40), pos + ImVec2(size.x + 100, 40), ImColor(1.f, 1.f, 1.f, 0.05f));

				switch (custom.m_tab) {
				case 0:
				{
					SetCursorPos(ImVec2(size.y - 370, 10));
					BeginGroup();
					{
						for (int i = 0; i < custom.legitbot_subtabs.size(); ++i) {

							if (custom.sub_tab(custom.legitbot_subtabs.at(i), custom.m_legitbot_subtab == i) && custom.m_legitbot_subtab != i)
								custom.m_legitbot_subtab = i, custom.m_anim = 0.f;

							if (i != custom.legitbot_subtabs.size() - 1)
								ImGui::SameLine();
						}
					}

					EndGroup();

					SetCursorPos(ImVec2(45, 50));
					BeginChild(X("##legitbot_childs"), ImVec2(GetWindowWidth() - 80, GetWindowHeight() - 80));
					{

						switch (custom.m_legitbot_subtab) {

						case 0:
						{
							custom.begin_child(X("General"), ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
							{
								ImGui::Checkbox(X("Enable"), &angel::_settings->aimbot);
								ImGui::Checkbox(X("Draw fov"), &angel::_settings->draw_fov);
								ImGui::SameLine(GetWindowWidth() - 33);
								ImGui::ColorEdit4(X("###fovcolor"), reinterpret_cast<float*>(&angel::_settings->fov_circle_color), ALPHA);
								ImGui::Checkbox(X("Only visible"), &angel::_settings->aimbot_visible_check);
								ImGui::Checkbox(X("Scope check"), &angel::_settings->aimbot_scopecheck);
								if (ImGui::Combo(X("Aimbot key"), &angel::_settings->aimbot_hotkey, aimbot_key_binds.data(), aimbot_key_binds.size()))
								{
									aimbot_t::set_hotkey(angel::_settings->aimbot_hotkey);
								}
								ImGui::Combo(X("Hitbox "), &angel::_settings->aim_position, hitboxes.data(), hitboxes.size());
								{
									switch (angel::_settings->aim_position)
									{
									case 0:
										angel::_settings->aim_hitbox_position = BONEINDEX::head;
										break;
									case 1:
										angel::_settings->aim_hitbox_position = BONEINDEX::neck_0;
										break;			
									case 2:				
										angel::_settings->aim_hitbox_position = BONEINDEX::spine_1;
										break;			
									case 3:				
										angel::_settings->aim_hitbox_position = BONEINDEX::spine_2;
										break;			
									case 4:				
										angel::_settings->aim_hitbox_position = BONEINDEX::pelvis;
										break;
									case 5:
										angel::_settings->aim_hitbox_position = 13;
										break;
									default:
										break;
									}
								}
								ImGui::SliderFloat(X("Fov"), &angel::_settings->aimbotfov, 0, 30);
								ImGui::SliderFloat(X("Smooth"), &aimbot_t::smooth, 0.1f, 30);
								ImGui::Checkbox(X("Enable rcs"), &angel::_settings->rcs);
								if (angel::_settings->rcs)
								{
									ImGui::SliderInt(X("Rcs bullet start"), &rcs::rcs_bullet, 0, 5);
									ImGui::SliderFloat(X("Rcs fov"), &angel::_settings->rcsfov, 0, 30);
								}

							} custom.end_child();

							ImGui::SameLine();

							custom.begin_child(X("Other stuff"), ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
							{
								ImGui::Checkbox(X("Enable weapon config"), &angel::_settings->weapon_config);
								if (angel::_settings->weapon_config)
								{
									ImGui::SliderFloat(X("Pistol fov"), &angel::_settings->pistol_fov, 0, 30);
									ImGui::SliderFloat(X("Pistol smooth"), &angel::_settings->pistol_smooth, 0.1f, 30);
									ImGui::SliderFloat(X("Rifle fov"), &angel::_settings->rifle_fov, 0, 30);
									ImGui::SliderFloat(X("Rifle smooth"), &angel::_settings->rifle_smooth, 0.1f, 30);
									ImGui::SliderFloat(X("Sniper fov"), &angel::_settings->sniper_fov, 0, 30);
									ImGui::SliderFloat(X("Sniper smooth"), &angel::_settings->sniper_smooth, 0.1f, 30);
									ImGui::SliderFloat(X("Heavy-smg fov"), &angel::_settings->heavysmg_fov, 0, 30);
									ImGui::SliderFloat(X("Heavy-smg smooth"), &angel::_settings->heavysmg_smooth, 0.1f, 30);
								}
								

							} custom.end_child();
						} break;


						case 1:
						{
							custom.begin_child(X("General"), ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
							{
								ImGui::Checkbox(X("Triggerbot"), &angel::_settings->triggerbot);
								if(ImGui::Combo(X("Triggerbot key"), &angel::_settings->triggerbot_hotkey, triggerbot_key_binds.data(), triggerbot_key_binds.size()))
								{
									triggerbot::set_hotkey(angel::_settings->triggerbot_hotkey);
								}
								ImGui::SliderInt(X("Shot delay"), &triggerbot::trigger_delay, 0, 1000);
								ImGui::SliderInt(X("Burst time"), &triggerbot::fake_shot_delay, 0, 1000);

							} custom.end_child();

							ImGui::SameLine();

							custom.begin_child(X("Other stuff"), ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
							{


							} custom.end_child();
						} break;
						case 2:
						{
							custom.begin_child(X("General"), ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
							{
								ImGui::Checkbox(X("Enable"), &angel::_settings->esp_enabled);
								ImGui::Checkbox(X("Visible check"), &angel::_settings->esp_visible_check);
								ImGui::SameLine(GetWindowWidth() - 33);
								ImGui::ColorEdit4(X("##EspVisCol"), reinterpret_cast<float*>(&angel::_settings->visible_color), ALPHA);
								ImGui::Checkbox(X("Bounding box"), &angel::_settings->box_esp);
								ImGui::SameLine(GetWindowWidth() - 33);
								ImGui::ColorEdit4(X("###boxcolor"), reinterpret_cast<float*>(&angel::_settings->box_color), ALPHA);
								ImGui::Checkbox(X("Outline"), &angel::_settings->outLine);
								if (angel::_settings->box_esp)
								{
									ImGui::Combo(X("Box style"), &angel::_settings->box_type, X("default\0edge\0corner\0corner edge\0"));
									ImGui::SliderFloat(X("Box rounding"), &angel::_settings->box_rounding, 0.1f, 3.0f, "%.1f", ALPHA);
								}
								ImGui::Checkbox(X("Filled Box"), &angel::_settings->filled_box);
								ImGui::SameLine(GetWindowWidth() - 49);
								ImGui::ColorEdit4(X("Filled box color"), reinterpret_cast<float*>(&angel::_settings->filled_color), ALPHA);
								ImGui::SameLine(GetWindowWidth() - 33);
								ImGui::ColorEdit4(X("Filled box visible color"), reinterpret_cast<float*>(&angel::_settings->filled_vis_color), ALPHA);
								ImGui::Checkbox(X("Filled Box gradient"), &angel::_settings->multi_color);
								ImGui::SameLine(GetWindowWidth() - 33);
								ImGui::ColorEdit4(X("###Filledboxgradientcolor"), reinterpret_cast<float*>(&angel::_settings->filled_color2), ALPHA);
								ImGui::Checkbox(X("Name"), &angel::_settings->player_name);
								ImGui::Checkbox(X("Health bar"), &angel::_settings->healthbar);
								ImGui::Checkbox(X("Ammo bar"), &angel::_settings->ammobar);
								ImGui::Checkbox(X("Weapon"), &angel::_settings->weapon_esp);
								ImGui::Checkbox(X("Distance"), &angel::_settings->show_distance);
								ImGui::Checkbox(X("Skeleton"), &angel::_settings->bone_esp);
								ImGui::SameLine(GetWindowWidth() - 33);
								ImGui::ColorEdit4(X("##BoneColor"), reinterpret_cast<float*>(&angel::_settings->bone_color), ALPHA);
							

							} custom.end_child();

							ImGui::SameLine();

							custom.begin_child(X("Other stuff"), ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
							{
								ImGui::Checkbox(X("Enable radar"), &angel::_settings->show_radar);
								if (angel::_settings->show_radar)
								{
									ImGui::Combo(X("Radar style"), &angel::_settings->radar_type, X("circle\0arrow\0both\0"));

									ImGui::Checkbox(X("Customize"), &angel::_settings->custom_radar);
									if (angel::_settings->custom_radar)
									{
										ImGui::SliderFloat(X("Point size"), &angel::_settings->radar_point_size_proportion, 0.5f, 2.f);
										ImGui::SliderFloat(X("Proportion"), &angel::_settings->proportion, 1200.f, 4500.f);
										ImGui::SliderFloat(X("Radar size"), &angel::_settings->radar_range, 100.f, 300.f);
										ImGui::SliderFloat(X("Background alpha"), &angel::_settings->radar_bg_alpha, 0.f, 1.f);
										ImGui::Checkbox(X("Crossline"), &angel::_settings->show_radar_crossline);
										if (angel::_settings->show_radar_crossline)
										{
											ImGui::SameLine(GetWindowWidth() - 33);
											ImGui::ColorEdit4(X("##CrossLineColor"), reinterpret_cast<float*>(&angel::_settings->radar_crossline_color), ALPHA);
										}
										}
								}

							} custom.end_child();
						} break;
						case 3:
						{
							custom.begin_child(X("General"), ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
							{
								ImGui::Checkbox(X("Bunnyhop (!)"), &angel::_settings->bunnyhop);
								ImGui::Checkbox(X("Cheat list"), &angel::_settings->cheat_list);
								ImGui::Checkbox(X("OBS bypass"), &angel::_settings->obsbypass);
								ImGui::Checkbox(X("Bomb timer"), &angel::_settings->bomb_timer);
								ImGui::SameLine(GetWindowWidth() - 33);
								ImGui::ColorEdit4(X("##BombTimerCol"), reinterpret_cast<float*>(&angel::_settings->bomb_timer_color), ALPHA);
								ImGui::Checkbox(X("Watermark"), &angel::_settings->watermark);
								ImGui::Checkbox(X("Discord rpc"), &angel::_settings->discordrpc);

							
							} custom.end_child();

							ImGui::SameLine();

							custom.begin_child("Other stuff", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
							{
								ImGui::Checkbox(X("Crosshair placement"), &angel::_settings->crosshair_placement);
								ImGui::Checkbox(X("Spectator check"), &angel::_settings->work_in_spec);
								ImGui::Checkbox(X("Team check"), &angel::_settings->team_check);
								ImGui::Checkbox(X("Hit sound"), &angel::_settings->hitsound);
								if (angel::_settings->hitsound)
									ImGui::Combo(X("Sound"), &angel::_settings->hitsound_type, hitsound.data(), hitsound.size());

							} custom.end_child();
						} break;
						case 4:
						{
							custom.begin_child(X("General"), ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
							{
								sprintf_s(TempText, "%s%s", ICON_FA_FOLDER_OPEN, X("config menu"));
								ConfigMenu::render_config_menu(TempText);

							} custom.end_child();

							ImGui::SameLine();

							custom.begin_child(X("Other stuff"), ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
							{
							} custom.end_child();
						} break;


						}

					} EndChild();

				} break;

				SetCursorPosY(0);
				custom.tab_area(X("##tab_area"), ImVec2(50, size.y - 20), []() 
					{

					for (int i = 0; i < custom.tabs.size(); ++i)
						if (custom.tab(custom.tabs_icons.at(i), custom.tabs.at(i), custom.m_tab == i) && custom.m_tab != i)
							custom.m_tab = i, custom.m_anim = 0.f;

					}

				);

				}ImGui::End();
				draw->AddRectFilled(pos + ImVec2(0, size.y - 20), pos + size, ImColor(15, 14, 21), style.WindowRounding, ImDrawFlags_RoundCornersBottom);
				draw->AddText(pos + ImVec2(5, size.y - 18), GetColorU32(ImGuiCol_Text), X("angeldancy for Counter-Strike 2"));
				ImGui::PopStyleVar(2);
			}
		}
	}
}
