#pragma once
#include "../../sdk/render/render.hpp"
#include "../../framework/cfg/cfg.hpp"
#include "../../hacks/ctx/hacks_ctx.h"
#include "../../hacks/features/menu/weaponicon.h"
#define ICON_FA_EYE "\xef\x81\xae"

namespace ESP
{
	struct WeaponIconSize
	{
		float width;
		float height;
		float offsetX;
		float offsetY;
	};

	std::unordered_map<std::string, WeaponIconSize> weaponIconSizes = 
	{
		{X("knife"), {20.0f, 20.0f, -8.0f, 0.0f}},
		{X("deagle"), {20.0f, 20.0f, -8.0f, 0.0f}},
		{X("elite"), {20.0f, 20.0f, 0.0f, 0.0f}},
		{X("fiveseven"), {20.0f, 20.0f, 0.0f, 0.0f}},
		{X("glock"), {20.0f, 20.0f, 0.0f, 0.0f}},
		{X("revolver"), {20.0f, 20.0f, -5.0f, 0.0f}},
		{X("hkp2000"), {20.0f, 20.0f, 0.0f, 0.0f}},
		{X("p250"), {20.0f, 20.0f, 0.0f, 0.0f}},
		{X("usp_silencer"), {20.0f, 20.0f, 0.0f, 0.0f}},
		{X("tec9"), {20.0f, 20.0f, 0.0f, 0.0f}},
		{X("cz75a"), {20.0f, 20.0f, 0.0f, 0.0f}},
		{X("mac10"), {20.0f, 20.0f, 0.0f, 0.0f}},
		{X("ump45"), {20.0f, 20.0f, -10.0f, 0.0f}},
		{X("bizon"), {20.0f, 20.0f, -10.0f, 0.0f}},
		{X("mp7"), {20.0f, 20.0f, -5.0f, 0.0f}},
		{X("mp9"), {20.0f, 20.0f, -10.0f, 0.0f}},
		{X("p90"), {20.0f, 20.0f, -10.0f, 0.0f}},
		{X("galilar"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("famas"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("m4a1_silencer"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("m4a1"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("aug"), {20.0f, 20.0f, -10.0f, 0.0f}},
		{X("sg556"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("ak47"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("g3sg1"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("scar20"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("awp"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("ssg08"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("xm1014"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("sawedoff"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("mag7"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("nova"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("negev"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("m249"), {20.0f, 20.0f, -15.0f, 0.0f}},
		{X("taser"), {20.0f, 20.0f, 0.0f, 0.0f}},
		{X("flashbang"), {20.0f, 20.0f, 5.0f, 0.0f}},
		{X("hegrenade"), {20.0f, 20.0f, 5.0f, 0.0f}},
		{X("smokegrenade"), {20.0f, 20.0f, 5.0f, 0.0f}},
		{X("molotov"), {20.0f, 20.0f, 5.0f, 0.0f}},
		{X("decoy"), {20.0f, 20.0f, 5.0f, 0.0f}},
		{X("incgrenade"), {20.0f, 20.0f, 5.0f, 0.0f}},
		{X("c4"), {20.0f, 20.0f, 0.0f, 0.0f}},
	};

	ImVec4 GetBoxRect(const CEntity& Entity, int BoxType)
	{
		ImVec4 Rect;
		switch (BoxType)
		{
		case 0:
			Rect = Render::Get2DBox(Entity);
			break;
		case 1:
			Rect = Render::Get2DBoneRect(Entity);
			break;
		case 2:
			Rect = Render::Get2DBox(Entity);
			break;
		case 3:
			Rect = Render::Get2DBox(Entity);
			break;
		case 4:
			Rect = Render::Get2DBoneRect(Entity);
			break;
		default:
			break;
		}

		return Rect;
	}

	const char* RenderWeaponIcon(const CEntity& Entity)
	{
		uintptr_t ClippingWeapon, WeaponData, WeaponNameAddress;
		_proc_manager.read_memory(Entity.Pawn.Address + Offset::Pawn.pClippingWeapon, ClippingWeapon);
		_proc_manager.read_memory(ClippingWeapon + 0x360, WeaponData);
		_proc_manager.read_memory(WeaponData + Offset::WeaponBaseData.szName, WeaponNameAddress);
		std::string weaponName = X("Invalid Weapon Name");

		if (!WeaponNameAddress)
		{
			weaponName = X("NULL");
		}
		else {
			weaponName = Entity.Pawn.WeaponName;
		}
		std::string weaponIcon = GunIcon(weaponName);
		return weaponIcon.c_str();
	}

	void RenderPlayerESP(const CEntity& LocalEntity, const CEntity& Entity, ImVec4 Rect, int LocalPlayerControllerIndex, int Index)
	{
		std::string weaponIcon = GunIcon(Entity.Pawn.WeaponName);

		Render::DrawBone(Entity, angel::_settings->bone_color, 1.1f);

		if (angel::_settings->filled_box)
		{
			float Rounding = angel::_settings->box_rounding;
				Rounding = 0.1f;
			ImColor FlatBoxCol = angel::_settings->filled_color;
			ImColor FlatBoxCol2 = angel::_settings->filled_color2;
			ImColor FlatBoxVisCol = angel::_settings->filled_vis_color;
			if (angel::_settings->filled_vis_box)
			{
				if ((Entity.Pawn.bSpottedByMask & (DWORD64(1) << LocalPlayerControllerIndex)) ||
					(LocalEntity.Pawn.bSpottedByMask & (DWORD64(1) << Index))) 
				{

					Gui.RectangleFilled({ Rect.x, Rect.y }, { Rect.z, Rect.w }, FlatBoxVisCol, Rounding);
				}
				else {
					Gui.RectangleFilled({ Rect.x, Rect.y }, { Rect.z, Rect.w }, FlatBoxCol, Rounding);
				}
			}
			else {
				if (angel::_settings->multi_color)
				{
					Gui.RectangleFilledGraident({ Rect.x, Rect.y }, { Rect.z, Rect.w }, angel::_settings->box_color, FlatBoxCol, FlatBoxCol2, Rounding);
				}
				else
				{
					Gui.RectangleFilled({ Rect.x, Rect.y }, { Rect.z, Rect.w }, FlatBoxCol, Rounding);
				}
				
			}
		}
		if (angel::_settings->box_esp)
		{	
			if (angel::_settings->box_type == 0 || angel::_settings->box_type == 1)
			{
				if (angel::_settings->outLine)
					Gui.Rectangle({ Rect.x,Rect.y }, { Rect.z,Rect.w }, angel::_settings->box_color & IM_COL32_A_MASK, 3, angel::_settings->box_rounding);

				if (((Entity.Pawn.bSpottedByMask & (DWORD64(1) << LocalPlayerControllerIndex)) || (LocalEntity.Pawn.bSpottedByMask & (DWORD64(1) << Index))) && angel::_settings->esp_visible_check)
				{
					Gui.Rectangle({ Rect.x,Rect.y }, { Rect.z,Rect.w }, angel::_settings->visible_color, 1.3, angel::_settings->box_rounding);
				}
				else {
					Gui.Rectangle({ Rect.x,Rect.y }, { Rect.z,Rect.w }, angel::_settings->box_color, 1.3, angel::_settings->box_rounding);
				}
			}
			else if (angel::_settings->box_type == 2 || angel::_settings->box_type == 3)
			{
				//Outline
				Gui.Line({ Rect.x, Rect.y }, { Rect.x + Rect.z * 0.25f, Rect.y }, angel::_settings->box_color & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x, Rect.y }, { Rect.x, Rect.y + Rect.w * 0.25f }, angel::_settings->box_color & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x + Rect.z, Rect.y + Rect.w }, { Rect.x + Rect.z - Rect.z * 0.25f, Rect.y + Rect.w }, angel::_settings->box_color & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x + Rect.z, Rect.y + Rect.w }, { Rect.x + Rect.z, Rect.y + Rect.w - Rect.w * 0.25f }, angel::_settings->box_color & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x, Rect.y + Rect.w }, { Rect.x + Rect.z * 0.25f, Rect.y + Rect.w }, angel::_settings->box_color & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x, Rect.y + Rect.w }, { Rect.x, Rect.y + Rect.w - Rect.w * 0.25f }, angel::_settings->box_color & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x + Rect.z, Rect.y }, { Rect.x + Rect.z - Rect.z * 0.25f, Rect.y }, angel::_settings->box_color & IM_COL32_A_MASK, 3);
				Gui.Line({ Rect.x + Rect.z, Rect.y }, { Rect.x + Rect.z, Rect.y + Rect.w * 0.25f }, angel::_settings->box_color & IM_COL32_A_MASK, 3);

				// Main Box Lines
				if (((Entity.Pawn.bSpottedByMask & (DWORD64(1) << LocalPlayerControllerIndex)) || (LocalEntity.Pawn.bSpottedByMask & (DWORD64(1) << Index))) && angel::_settings->esp_visible_check)
				{
					Gui.Line({ Rect.x, Rect.y }, { Rect.x + Rect.z * 0.25f, Rect.y }, angel::_settings->visible_color, 1.3f);
					Gui.Line({ Rect.x, Rect.y }, { Rect.x, Rect.y + Rect.w * 0.25f }, angel::_settings->visible_color, 1.3f);
					Gui.Line({ Rect.x + Rect.z, Rect.y }, { Rect.x + Rect.z - Rect.z * 0.25f, Rect.y }, angel::_settings->visible_color, 1.3f);
					Gui.Line({ Rect.x + Rect.z, Rect.y }, { Rect.x + Rect.z, Rect.y + Rect.w * 0.25f }, angel::_settings->visible_color, 1.3f);
					Gui.Line({ Rect.x, Rect.y + Rect.w }, { Rect.x + Rect.z * 0.25f, Rect.y + Rect.w }, angel::_settings->visible_color, 1.3f);
					Gui.Line({ Rect.x, Rect.y + Rect.w }, { Rect.x, Rect.y + Rect.w - Rect.w * 0.25f }, angel::_settings->visible_color, 1.3f);
					Gui.Line({ Rect.x + Rect.z, Rect.y + Rect.w }, { Rect.x + Rect.z - Rect.z * 0.25f, Rect.y + Rect.w }, angel::_settings->visible_color, 1.3f);
					Gui.Line({ Rect.x + Rect.z, Rect.y + Rect.w }, { Rect.x + Rect.z, Rect.y + Rect.w - Rect.w * 0.25f }, angel::_settings->visible_color, 1.3f);
				}
				else {
					Gui.Line({ Rect.x, Rect.y }, { Rect.x + Rect.z * 0.25f, Rect.y }, angel::_settings->box_color, 1.3f);
					Gui.Line({ Rect.x, Rect.y }, { Rect.x, Rect.y + Rect.w * 0.25f }, angel::_settings->box_color, 1.3f);
					Gui.Line({ Rect.x + Rect.z, Rect.y }, { Rect.x + Rect.z - Rect.z * 0.25f, Rect.y }, angel::_settings->box_color, 1.3f);
					Gui.Line({ Rect.x + Rect.z, Rect.y }, { Rect.x + Rect.z, Rect.y + Rect.w * 0.25f }, angel::_settings->box_color, 1.3f);
					Gui.Line({ Rect.x, Rect.y + Rect.w }, { Rect.x + Rect.z * 0.25f, Rect.y + Rect.w }, angel::_settings->box_color, 1.3f);
					Gui.Line({ Rect.x, Rect.y + Rect.w }, { Rect.x, Rect.y + Rect.w - Rect.w * 0.25f }, angel::_settings->box_color, 1.3f);
					Gui.Line({ Rect.x + Rect.z, Rect.y + Rect.w }, { Rect.x + Rect.z - Rect.z * 0.25f, Rect.y + Rect.w },angel::_settings->box_color, 1.3f);
					Gui.Line({ Rect.x + Rect.z, Rect.y + Rect.w }, { Rect.x + Rect.z, Rect.y + Rect.w - Rect.w * 0.25f },angel::_settings->box_color, 1.3f);
				}
			}
		}


		if (angel::_settings->weapon_esp)
		{
			if (angel::_settings->healthbar_type == 0 || angel::_settings->healthbar_type == 1)
			{
				WeaponIconSize iconSize = weaponIconSizes[Entity.Pawn.WeaponName];
				ImVec2 textPosition = { Rect.x + (Rect.z - iconSize.width) / 2 + iconSize.offsetX, Rect.y + Rect.w + 1 + iconSize.offsetY};
				if (angel::_settings->ammobar)
					textPosition.y += 5;
				// Gui.StrokeText(Entity.Pawn.WeaponName, { Rect.x + Rect.z / 2,Rect.y + Rect.w + 10}, ImColor(255, 255, 255, 255), 14, true);
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 15.0f, ImVec2{ textPosition.x - 1, textPosition.y - 1 }, ImColor(0, 0, 0, 255), weaponIcon.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 15.0f, ImVec2{ textPosition.x - 1, textPosition.y + 1 }, ImColor(0, 0, 0, 255), weaponIcon.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 15.0f, ImVec2{ textPosition.x + 1, textPosition.y + 1 }, ImColor(0, 0, 0, 255), weaponIcon.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 15.0f, ImVec2{ textPosition.x + 1, textPosition.y - 1 }, ImColor(0, 0, 0, 255), weaponIcon.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 15.0f, textPosition, ImColor(255, 255, 255, 255), weaponIcon.c_str());
			}
		}

		if (angel::_settings->scoped_esp)
		{
			bool is_scoped;
			ImVec2 IconPos = { Rect.x, Rect.y };
			_proc_manager.read_memory<bool>(Entity.Pawn.Address + Offset::Pawn.isScoped, is_scoped);
			if (is_scoped)
			{
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 13.0f, ImVec2{ IconPos.x - 1, IconPos.y - 1 }, ImColor(0, 0, 0, 255), "s");
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 13.0f, ImVec2{ IconPos.x - 1, IconPos.y + 1 }, ImColor(0, 0, 0, 255), "s");
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 13.0f, ImVec2{ IconPos.x + 1, IconPos.y + 1 }, ImColor(0, 0, 0, 255), "s");
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 13.0f, ImVec2{ IconPos.x + 1, IconPos.y - 1 }, ImColor(0, 0, 0, 255), "s");
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetIO().Fonts->Fonts[1], 13.0f, IconPos, ImColor(0, 200, 255, 255), "s");
			}
		}
			
		if (angel::_settings->player_name)
		{
			if (angel::_settings->healthbar_type == 0)
				Gui.StrokeText(Entity.Controller.PlayerName, { Rect.x + Rect.z / 2,Rect.y - 14 }, ImColor(255, 255, 255, 255), 14, true);
			else
				Gui.StrokeText(Entity.Controller.PlayerName, { Rect.x + Rect.z / 2,Rect.y - 13 - 14 }, ImColor(255, 255, 255, 255), 14, true);
		}
	}
}
