#include <string>
#include <thread>
#include <future>
#include <iostream>

#include "hacks_ctx.h"
#include "../../sdk/render/render.hpp"
#include "../../framework/cfg/cfg.hpp"

#include "../../hacks/features/visuals/esp.h"
#include "../../hacks/features/menu/gui.h"
#include "../../hacks/features/rcs/rcs.h"
#include "../../hacks/features/misc/bombtimer.h"

int PreviousTotalHits = 0;

void cheats::radar_setting(Base_Radar& Radar)
{
	ImGui::SetNextWindowBgAlpha(angel::_settings->radar_bg_alpha);
	ImGui::Begin(X("Radar"), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize({ angel::_settings->radar_range * 2,angel::_settings->radar_range * 2 });
	
	if (!angel::_settings->custom_radar)
	{
		ImGui::SetWindowPos(ImVec2(0, 0));
		angel::_settings->show_radar_crossline = false;
		angel::_settings->proportion = 3300.f;
		angel::_settings->radar_point_size_proportion = 1.f;
		angel::_settings->radar_range = 150.f;
		angel::_settings->radar_bg_alpha = 0.1f;
	}
	
	// Radar.SetPos({ Gui.Window.Size.x / 2,Gui.Window.Size.y / 2 });
	Radar.SetDrawList(ImGui::GetWindowDrawList());
	Radar.SetPos({ ImGui::GetWindowPos().x + angel::_settings->radar_range, ImGui::GetWindowPos().y + angel::_settings->radar_range });
	Radar.SetProportion(angel::_settings->proportion);
	Radar.SetRange(angel::_settings->radar_range);
	Radar.SetSize(angel::_settings->radar_range * 2);
	Radar.SetCrossColor(angel::_settings->radar_crossline_color);

	Radar.ArcArrowSize *= angel::_settings->radar_point_size_proportion;
	Radar.ArrowSize *= angel::_settings->radar_point_size_proportion;
	Radar.CircleSize *= angel::_settings->radar_point_size_proportion;

	Radar.ShowCrossLine = angel::_settings->radar_crossline_color;
	Radar.Opened = true;
}

void cheats::run()
{	
	static DWORD lastTick = 0; 
	DWORD currentTick = GetTickCount64(); 
	if ((GetAsyncKeyState(VK_INSERT) & 0x8000) && currentTick - lastTick >= 150) 
	{
		angel::_settings->show_menu = !angel::_settings->show_menu;
		lastTick = currentTick;
	}

	if (angel::_settings->show_menu)
	{
		gui::main_menu();
	}
		
	if(!_proc_manager.read_memory(g_game.get_matrix_address(), g_game.view.Matrix,64))
		return;

	g_game.update_entity_list_entry();

	DWORD64 LocalControllerAddress = 0;
	DWORD64 LocalPawnAddress = 0;

	if (!_proc_manager.read_memory(g_game.get_local_controller_address(), LocalControllerAddress))
		return;
	if (!_proc_manager.read_memory(g_game.get_local_pawn_address(), LocalPawnAddress))
		return;

	CEntity LocalEntity, ServerEntity;
	static int LocalPlayerControllerIndex = 1;
	if (!LocalEntity.UpdateController(LocalControllerAddress))
		return;
	if (!LocalEntity.UpdatePawn(LocalPawnAddress) && !angel::_settings->work_in_spec)
		return;

	// HealthBar Map
	static std::map<DWORD64, Render::HealthBar> HealthBarMap;

	// AimBot data
	float DistanceToSight = 0;
	float MaxAimDistance = 100000;
	Vec3  HeadPos{ 0,0,0 };
	Vec3  AimPos{ 0,0,0 };
	Vec2  Angles{ 0,0 };

	// Radar Data
	Base_Radar Radar;
	if (angel::_settings->show_radar)
		radar_setting(Radar);

	// skip first entity, it's always world??
	for (int i = 0; i < 64; i++)
	{
		CEntity Entity;
		DWORD64 EntityAddress = 0;
		if (!_proc_manager.read_memory<DWORD64>(g_game.get_entity_list_entry() + (i + 1) * 0x78, EntityAddress))
			continue;
		if (EntityAddress == LocalEntity.Controller.Address)
		{
			LocalPlayerControllerIndex = i;
			continue;
		}
		if (!Entity.UpdateController(EntityAddress))
			continue;

		if (!Entity.UpdatePawn(Entity.Pawn.Address))
			continue;

		if (angel::_settings->team_check && Entity.Controller.TeamID == LocalEntity.Controller.TeamID)
			continue;

		if (!Entity.IsAlive())
			continue;
//		if (menuconfig::VisibleCheck && (!Entity.Pawn.bSpottedByMask > 0))
//			continue;

		if (angel::_settings->show_radar)
			Radar.AddPoint(LocalEntity.Pawn.Pos, LocalEntity.Pawn.ViewAngle.y, Entity.Pawn.Pos, ImColor(237, 85, 106, 200), angel::_settings->radar_type, Entity.Pawn.ViewAngle.y);

		if (!Entity.IsInScreen())
			continue;

		DistanceToSight = Entity.GetBone().BonePosList[BONEINDEX::head].ScreenPos.DistanceTo({ Gui.Window.Size.x / 2,Gui.Window.Size.y / 2 });

		if (DistanceToSight < MaxAimDistance)
		{
			MaxAimDistance = DistanceToSight;

			if (!angel::_settings->aimbot_visible_check ||
				Entity.Pawn.bSpottedByMask & (DWORD64(1) << (LocalPlayerControllerIndex)) ||
				LocalEntity.Pawn.bSpottedByMask & (DWORD64(1) << (i)))
			{
				AimPos = Entity.GetBone().BonePosList[angel::_settings->aim_hitbox_position].Pos;
				if (angel::_settings->aim_hitbox_position == BONEINDEX::head)
					AimPos.z -= 1.f;
			}
		}

		if (angel::_settings->esp_enabled)
		{
			ImVec4 Rect = ESP::GetBoxRect(Entity, angel::_settings->box_type);
			ESP::RenderPlayerESP(LocalEntity, Entity, Rect, LocalPlayerControllerIndex, i);
			
			if (angel::_settings->healthbar)
			{
				ImVec2 HealthBarPos = { Rect.x - 6.5f,Rect.y };
				ImVec2 HealthBarSize = { 3.5f ,Rect.w };
				Render::DrawHealthBar(EntityAddress, 100, Entity.Pawn.Health, HealthBarPos, HealthBarSize);
			}

			if (angel::_settings->ammobar && Entity.Pawn.Ammo != -1)
			{
				ImVec2 AmmoBarPos = { Rect.x, Rect.y + Rect.w + 2 };
				ImVec2 AmmoBarSize = { Rect.z,4 };
				Render::DrawAmmoBar(EntityAddress, Entity.Pawn.MaxAmmo, Entity.Pawn.Ammo, AmmoBarPos, AmmoBarSize);

			}

			Render::DrawDistance(LocalEntity, Entity, Rect);
		}
	}

	if(angel::_settings->show_radar)
	{
		Radar.Render();
		ImGui::End();
	}

	if (angel::_settings->triggerbot && (GetAsyncKeyState(triggerbot::hotkey) || angel::_settings->triggerbot_always))
		triggerbot::run(LocalEntity);	

	misc::hit_sound(LocalEntity, PreviousTotalHits);
	misc::watermark();
	misc::cheat_list();	

	Render::HeadShootLine(LocalEntity, angel::_settings->crosshair_placement_color);
	
	triggerbot::target_check(LocalEntity);

	bunnyhop::run(LocalEntity);

	bombtimer::render_window();

	if (angel::_settings->aimbot)
	{
		Render::DrawFovCircle(LocalEntity);

		if (angel::_settings->aimbot_always)
		{
				aimbot_t::aimbot(LocalEntity, LocalEntity.Pawn.CameraPos, AimPos);
		}
		else
		{
			if (GetAsyncKeyState(aimbot_t::hotkey))
			{
					aimbot_t::aimbot(LocalEntity, LocalEntity.Pawn.CameraPos, AimPos);
			}
		}

		if (angel::_settings->aimbot_toggle_mode && (GetAsyncKeyState(aimbot_t::hotkey) & 0x8000) && currentTick - lastTick >= 200)
		{
			aimbot_t::switch_toggle();
			lastTick = currentTick;
		}
	}		
}
