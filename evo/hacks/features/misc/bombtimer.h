#pragma once
#include <chrono>
#include <iostream>
#include <utility>
#include <sstream>
#include <ctime>
#include <string>
#include "../../sdk/classes/entity.h"
#include "../../framework/cfg/cfg.hpp"

namespace bombtimer
{
	bool is_planted = false;
	std::time_t planttime;

	uint64_t current_time_millis() {
		using namespace std::chrono;
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}

	int get_bomb_site(bool planted)
	{
		if (planted)
		{
			int site;
			uintptr_t cplantedc4;
			_proc_manager.read_memory(g_game.get_client_dll_address() + Offset::PlantedC4, cplantedc4);
			if (!_proc_manager.read_memory<uintptr_t>(g_game.get_client_dll_address() + Offset::PlantedC4, cplantedc4))
				return 0;
			if (!_proc_manager.read_memory<uintptr_t>(cplantedc4, cplantedc4))
				return 0;

			if (!_proc_manager.read_memory<int>(cplantedc4 + Offset::C4.m_nBombSite, site))
				return 0;

			return site;
		}
		
	}

	void render_window()
	{
		if (!angel::_settings->bomb_timer)
			return;

		bool is_bomb_planted;
		bool is_being_defused;
		float defusetime;
		auto planted_address = g_game.get_client_dll_address() + Offset::PlantedC4 - 0x8;

		static float windowWidth = 190.0f;
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiColorEditFlags_NoBorder;
		ImGui::SetNextWindowPos({ (ImGui::GetIO().DisplaySize.x - 200.0f) / 2.0f, 80.0f }, ImGuiCond_Once);
		ImGui::SetNextWindowSize({ windowWidth, 0 }, ImGuiCond_Once);
		if (!angel::_settings->show_menu)
			ImGui::SetNextWindowBgAlpha(0.3f);
		ImGui::Begin(X("Bomb Timer"), nullptr, flags);

		_proc_manager.read_memory(planted_address, is_bomb_planted);

		_proc_manager.read_memory(Offset::PlantedC4 + Offset::C4.m_bBeingDefused, is_being_defused);
		_proc_manager.read_memory(Offset::PlantedC4 + Offset::C4.m_flDefuseCountDown, defusetime);

		auto time = current_time_millis();

		if (is_bomb_planted && !is_planted && (planttime == NULL || time - planttime > 60000))
		{
			is_planted = true;
			planttime = time;
		}

		float remaining = (40000 - (int64_t)time + planttime) / (float)1000;

		
		if (remaining > 10 || remaining < 0 || !is_planted)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 196, 0, 255));
		}
		else if (remaining > 5)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 155, 0, 255));
		}
		else 
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(242, 93, 93, 255));
		}

		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 180) * 0.5f);
		float barLength = remaining <= 0.0f ? 0.0f : remaining >= 40 ? 1.0f : (remaining / 40.0f);
		
		if (is_planted && remaining >= 0)
		{
			std::ostringstream ss;
			ss.precision(3);
			ss << "Bomb on " << (!get_bomb_site(is_bomb_planted) ? "A" : "B") << ": " << std::fixed << remaining << " s";
			Gui.MyText(std::move(ss).str().c_str(), true);
		}
		else 
		{
			Gui.MyText(X("C4 not planted"), true);
			barLength = 0.0f;
		}

		Gui.MyProgressBar(barLength, { 180, 15 }, "", angel::_settings->bomb_timer_color);

		if (is_planted && !is_bomb_planted)
		{
			is_planted = false;
		}

		ImGui::PopStyleColor();
		ImGui::End();
	}
}