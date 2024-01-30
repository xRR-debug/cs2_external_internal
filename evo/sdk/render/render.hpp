#pragma once
#define _USE_MATH_DEFINES
#define IMGUI_DEFINE_MATH_OPERATORS
#include <math.h>
#include <chrono>
#include <map>
#include <Windows.h>
#include "../../sdk/classes/entity.h"
#include "../../utils/format.h"
#include "../../utils/os-imgui/imgui/imgui.h"
#include "../../utils/os-imgui/imgui/imgui_internal.h"
#include "../../hacks/features/triggerbot/triggerbot.h"
#include "render_sdk.hpp"
#include "../../hacks/features/aimbot/aimbot.hpp"
#include "../../sdk/animation_system/animation_system.hpp"

namespace Render
{
	void add_gradient_horizontal(int x, int y, int w, int h, angel::col_t c1, angel::col_t c2) {
		ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(angel::macros::vec_t(x, y), angel::macros::vec_t(x + w, y + h), c1.convert(), c1.convert(), c2.convert(), c2.convert());
	}

	angel::col_t to_main_color(float color[4]) {
		return angel::col_t(color[0] * 255.f, color[1] * 255.f, color[2] * 255.f, color[3] * 255.f);
	}

	void DrawDistance(const CEntity& LocalEntity, CEntity& Entity, ImVec4 Rect)
	{
		if (!angel::_settings->show_distance)
			return;

		int distance = static_cast<int>(Entity.Pawn.Pos.DistanceTo(LocalEntity.Pawn.Pos) / 100);
		std::string dis_str = Format("%im", distance);
		Gui.StrokeText(dis_str, { Rect.x + Rect.z + 4, Rect.y }, ImColor(255, 204, 0, 255), 14, false);
	}

	void DrawFovCircle(const CEntity& LocalEntity)
	{
		if (!angel::_settings->draw_fov)
			return;

		Vec2 CenterPoint = Gui.Window.Size / 2;

		float Radius = tan(aimbot_t::AimFov / 180.f * M_PI / 2.f) / tan(LocalEntity.Pawn.Fov / 180.f * M_PI / 2.f) * Gui.Window.Size.x;
		Gui.Circle(CenterPoint, Radius, angel::_settings->fov_circle_color, 1);
	}

	void HeadShootLine(const CEntity& LocalEntity, ImColor Color)
	{
		if (!angel::_settings->crosshair_placement)
			return;

		Vec2 Pos;
		Pos.x = Gui.Window.Size.x / 2;
		Pos.y = Gui.Window.Size.y / 2.0f - Gui.Window.Size.y / (2.0f * std::sin(LocalEntity.Pawn.Fov * M_PI / 180.0f) / std::sin(90.0f * M_PI / 180.0f)) * std::sin(LocalEntity.Pawn.ViewAngle.x * M_PI / 180.0f) / std::sin(90.0f * M_PI / 180.0f);

		Gui.RectangleFilled(Vec2{ Pos.x - 21, Pos.y - 1 }, Vec2{ 17, 3}, Color & IM_COL32_A_MASK);
		Gui.RectangleFilled(Vec2{ Pos.x - 20, Pos.y }, Vec2{ 17, 3 }, Color);

		Gui.RectangleFilled(Vec2{ Pos.x + 5, Pos.y - 1 }, Vec2{ 17, 3 }, Color & IM_COL32_A_MASK);
		Gui.RectangleFilled(Vec2{ Pos.x + 6, Pos.y }, Vec2{ 17, 3 }, Color);
	}

	ImVec4 Get2DBox(const CEntity& Entity)
	{
		BoneJointPos Head = Entity.GetBone().BonePosList[BONEINDEX::head];

		Vec2 Size, Pos;
		Size.y = (Entity.Pawn.ScreenPos.y - Head.ScreenPos.y) * 1.09;
		Size.x = Size.y * 0.6;

		Pos = ImVec2(Entity.Pawn.ScreenPos.x - Size.x / 2, Head.ScreenPos.y- Size.y*0.08);

		return ImVec4{ Pos.x,Pos.y,Size.x,Size.y };
	}

	void DrawBone(const CEntity& Entity, ImColor Color, float Thickness)
	{
		if (!angel::_settings->bone_esp)
			return;

		BoneJointPos Previous, Current;
		for (auto i : BoneJointList::List)
		{
			Previous.Pos = Vec3(0, 0, 0);
			for (auto Index : i)
			{
				Current = Entity.GetBone().BonePosList[Index];
				if (Previous.Pos == Vec3(0, 0, 0))
				{
					Previous = Current;
					//pGame->View->Gui->Text(Current.Name.c_str(), Current.ScreenPos, ImColor(255, 255, 0, 255));
					continue;
				}
				if (Previous.IsVisible && Current.IsVisible)
				{
					Gui.Line(Previous.ScreenPos, Current.ScreenPos, Color, Thickness);
					//pGame->View->Gui->Text(Current.Name.c_str(), Current.ScreenPos, ImColor(255, 255, 0, 255));
				}
				Previous = Current;
			}
		}
	}

	ImVec4 Get2DBoneRect(const CEntity& Entity)
	{
		Vec2 Min, Max, Size;
		Min = Max = Entity.GetBone().BonePosList[0].ScreenPos;

		for (auto &BoneJoint : Entity.GetBone().BonePosList)
		{
			if (!BoneJoint.IsVisible)
				continue;
			Min.x = min(BoneJoint.ScreenPos.x, Min.x);
			Min.y = min(BoneJoint.ScreenPos.y, Min.y);
			Max.x = max(BoneJoint.ScreenPos.x, Max.x);
			Max.y = max(BoneJoint.ScreenPos.y, Max.y);
		}
		Size.x = Max.x - Min.x;
		Size.y = Max.y - Min.y;

		return ImVec4(Min.x, Min.y, Size.x, Size.y);
	}

	class HealthBar
	{
	private:
		using TimePoint_ = std::chrono::steady_clock::time_point;
	private:
		const int ShowBackUpHealthDuration = 500;
		float MaxHealth = 0.f;
		float CurrentHealth = 0.f;
		float LastestBackupHealth = 0.f;
		ImVec2 RectPos{};
		ImVec2 RectSize{};
		bool InShowBackupHealth = false;
		TimePoint_ BackupHealthTimePoint{};
		int MaxAmmo = 0;
		int CurrentAmmo = 0;

	public:
		HealthBar() {}

		void HealthBarV(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size);

		void AmmoBarH(float MaxAmmo, float CurrentAmmo, ImVec2 Pos, ImVec2 Size);
	private:

		ImColor Mix(ImColor Col_1, ImColor Col_2, float t);

		ImColor FirstStageColor = ImColor(0, 255, 0, 255);

		ImColor SecondStageColor = ImColor(255, 232, 0, 255);

		ImColor ThirdStageColor = ImColor(255, 39, 0, 255);

		ImColor BackupHealthColor = ImColor(255, 255, 255, 220);

		ImColor FrameColor = ImColor(45, 45, 45, 220);

		ImColor BackGroundColor = ImColor(0, 0, 0, 255);

		ImColor AmmoColor = ImColor(255, 255, 0, 255);
	};

	void HealthBar::HealthBarV(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size)
	{
		auto InRange = [&](float value, float min, float max) -> bool
			{
				return value > min && value <= max;
			};

		ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

		this->MaxHealth = MaxHealth;
		this->CurrentHealth = CurrentHealth;
		this->RectPos = Pos;
		this->RectSize = Size;

		float Proportion = CurrentHealth / MaxHealth;

		float Height = RectSize.y * Proportion;

		ImColor Color;

		DrawList->AddRectFilled(RectPos,
			{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
			BackGroundColor, 5, 15);

		float Color_Lerp_t = pow(Proportion, 2.5);
		if (InRange(Proportion, 0.5, 1))
			Color = Mix(FirstStageColor, SecondStageColor, Color_Lerp_t * 3 - 1);
		else
			Color = Mix(SecondStageColor, ThirdStageColor, Color_Lerp_t * 4);

		DrawList->AddRectFilled({ RectPos.x,RectPos.y + RectSize.y - Height },
			{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
			Color, 0);

		DrawList->AddRect(RectPos,
			{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
			FrameColor, 0, 15, 1);
	}

	void HealthBar::AmmoBarH(float MaxAmmo, float CurrentAmmo, ImVec2 Pos, ImVec2 Size)
	{
		ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

		this->MaxAmmo = MaxAmmo;
		this->CurrentAmmo = CurrentAmmo;
		this->RectPos = Pos;
		this->RectSize = Size;

		float Proportion = CurrentAmmo / MaxAmmo;

		float Width = RectSize.x * Proportion;

		DrawList->AddRectFilled(RectPos,
			{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
			BackGroundColor, 5, 15);

		DrawList->AddRectFilled(RectPos,
			{ RectPos.x + Width,RectPos.y + RectSize.y },
			AmmoColor, 0);

		DrawList->AddRect(RectPos,
			{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
			FrameColor, 0, 15, 1);
	}

	ImColor HealthBar::Mix(ImColor Col_1, ImColor Col_2, float t)
	{
		ImColor Col;
		Col.Value.x = t * Col_1.Value.x + (1 - t) * Col_2.Value.x;
		Col.Value.y = t * Col_1.Value.y + (1 - t) * Col_2.Value.y;
		Col.Value.z = t * Col_1.Value.z + (1 - t) * Col_2.Value.z;
		Col.Value.w = Col_1.Value.w;
		return Col;
	}

	void DrawHealthBar(DWORD Sign, float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size)
	{
		static std::map<DWORD, HealthBar> HealthBarMap;
		if (!HealthBarMap.count(Sign))
			HealthBarMap.insert({ Sign,HealthBar() });

		if (HealthBarMap.count(Sign))
			HealthBarMap[Sign].HealthBarV(MaxHealth, CurrentHealth, Pos, Size);
	}

	void DrawAmmoBar(DWORD Sign, float MaxAmmo, float CurrentAmmo, ImVec2 Pos, ImVec2 Size)
	{
		static std::map<DWORD, HealthBar> HealthBarMap;
		if (!HealthBarMap.count(Sign))
			HealthBarMap.insert({ Sign,HealthBar() });

		if (HealthBarMap.count(Sign))
			HealthBarMap[Sign].AmmoBarH(MaxAmmo, CurrentAmmo, Pos, Size);
	}

	ImVec2 GetScreenCenterImVec2()
	{
		int W = GetSystemMetrics(SM_CXSCREEN);
		int H = GetSystemMetrics(SM_CYSCREEN);

		float CenterX = static_cast<float>(W) / 2;
		float CenterY = static_cast<float>(H) / 2;

		return ImVec2(CenterX, CenterY);
	}

	ImVec4 rgba2ImVec(int r, int g, int b, int a)
	{
		ImVec4 color;
		color.x = static_cast<float>(r) / 255.0f;
		color.y = static_cast<float>(g) / 255.0f;
		color.z = static_cast<float>(b) / 255.0f;
		color.w = static_cast<float>(a) / 255.0f;

		return color;
	}
}