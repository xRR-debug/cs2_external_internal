#include "misc.h"
#include <iostream>
#include <Shellapi.h>
#include "../../utils/hitsounds.hpp"

namespace misc
{
	void cheat_list() noexcept
	{
		if (!angel::_settings->cheat_list)
			return;

		ImGuiWindowFlags windowflags = ImGuiWindowFlags_NoCollapse;
		ImGui::SetNextWindowBgAlpha(0.3f);
		ImGui::SetNextWindowSize(ImVec2(200, 0));
		ImGui::Begin(X("               cheats list"), nullptr, windowflags);

		cheat_text(X("Visuals"), angel::_settings->esp_enabled);
		if (angel::_settings->aimbot && (angel::_settings->aimbot_always || GetAsyncKeyState(aimbot_t::hotkey)))
			ImGui::Text(X("Aimbot [Toggle]"));
		cheat_text(X("Recoil control"), angel::_settings->rcs);
		cheat_text(X("Radar"), angel::_settings->show_radar);
		if (angel::_settings->triggerbot && (angel::_settings->triggerbot_always || GetAsyncKeyState(angel::_settings->triggerbot_hotkey)))
			ImGui::Text(X("TriggerBot [Toggle]"));

		cheat_text(X("Crosshair placement"), angel::_settings->crosshair_placement);
		cheat_text(X("Bunnyhop"), angel::_settings->bunnyhop);
		cheat_text(X("HitSound"), angel::_settings->hitsound);
		cheat_text(X("Bomb Timer"), angel::_settings->bomb_timer);

		ImGui::End();
	}

	void watermark() noexcept
	{
		if (!angel::_settings->watermark)
			return;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::SetNextWindowBgAlpha(0.3f);
		ImGui::Begin(X("watermark"), nullptr, window_flags);

		static auto framerate = 1.0f;
		struct tm ptm;
		framerate = ImGui::GetIO().Framerate;
		get_current_time(&ptm);

		ImGui::Text(X("angeldancy | %d fps | %02d:%02d:%02d"),
			framerate != 0.0f ? static_cast<int>(framerate) : 0,
			ptm.tm_hour, ptm.tm_min, ptm.tm_sec);
		ImGui::End();
	}

	void hit_sound(const CEntity& aLocalPlayer, int& PreviousTotalHits) noexcept
	{
		if (!angel::_settings->hitsound)
			return;

		int total_hits;
		uintptr_t bullet_services;
		_proc_manager.read_memory(aLocalPlayer.Pawn.Address + Offset::Pawn.BulletServices, bullet_services);
		_proc_manager.read_memory(bullet_services + Offset::Pawn.TotalHit, total_hits);

		if (total_hits != PreviousTotalHits) {
			if (total_hits == 0 && PreviousTotalHits != 0) {} else
				if (angel::_settings->hitsound)
				{
					switch (angel::_settings->hitsound_type) {
					case 0:
					{
						PlaySoundA(reinterpret_cast<char*> (neverlose_sound), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					case 1:
					{
						PlaySoundA(reinterpret_cast<char*> (skeet_sound), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					case 2:
					{
						PlaySoundA(reinterpret_cast<char*> (primordial_sound), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					case 3:
					{
						PlaySoundA(reinterpret_cast<char*> (cock_sound), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					case 4:
					{
						PlaySoundA(reinterpret_cast<char*> (cookie), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					case 5:
					{
						PlaySoundA(reinterpret_cast<char*> (moan), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					case 6:
					{
						PlaySoundA(reinterpret_cast<char*> (rifk), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					case 7:
					{
						PlaySoundA(reinterpret_cast<char*> (roblox), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					case 8:
					{
						PlaySoundA(reinterpret_cast<char*> (button_snd), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					case 9:
					{
						PlaySoundA(reinterpret_cast<char*> (popsnd), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					case 10:
					{
						PlaySoundA(reinterpret_cast<char*> (wowsnd), NULL, SND_ASYNC | SND_MEMORY);
					} break;
					}
				}
		}
		PreviousTotalHits = total_hits;
	}
}
