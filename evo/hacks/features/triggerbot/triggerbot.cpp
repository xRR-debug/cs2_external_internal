#include "triggerbot.h"

DWORD uHandle = 0;
DWORD64 ListEntry = 0;
DWORD64 PawnAddress = 0;
CEntity Entity;
bool allow_shoot = false;

void triggerbot::release_mouse_event()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(fake_shot_delay));
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void triggerbot::run(const CEntity& LocalEntity)
{
	if (!_proc_manager.read_memory<DWORD>(LocalEntity.Pawn.Address + Offset::Pawn.iIDEntIndex, uHandle))
		return;
	if (uHandle == -1)
		return;

	ListEntry = _proc_manager.trace_address(g_game.get_entity_list_address(), { 0x8 * (uHandle >> 9) + 0x10,0x0 });
	if (ListEntry == 0)
		return;

	if (!_proc_manager.read_memory<DWORD64>(ListEntry + 0x78 * (uHandle & 0x1FF), PawnAddress))
		return;

	if (!Entity.UpdatePawn(PawnAddress))
		return;

	if (LocalEntity.Pawn.weapon_type == cs_weapon_type::weapon_type_knife)
		return;
	if (LocalEntity.Pawn.weapon_type == cs_weapon_type::weapon_type_grenade)
		return;
	if (LocalEntity.Pawn.weapon_type == cs_weapon_type::weapon_type_c4)
		return;

	if (angel::_settings->team_check)
		allow_shoot = LocalEntity.Pawn.TeamID != Entity.Pawn.TeamID && Entity.Pawn.Health > 0;
	else
		allow_shoot = Entity.Pawn.Health > 0;

	if (!allow_shoot)
		return;

	static std::chrono::time_point last_point = std::chrono::steady_clock::now();
	auto cur_point = std::chrono::steady_clock::now();
	if (cur_point - last_point >= std::chrono::milliseconds(trigger_delay))
	{
		const bool shooting = GetAsyncKeyState(VK_LBUTTON) < 0;
		if (!shooting)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			std::thread trigger_thread(release_mouse_event);
			trigger_thread.detach();
		}
		last_point = cur_point;
	}
}

void triggerbot::target_check(const CEntity& LocalEntity) noexcept
{
	if (!_proc_manager.read_memory<DWORD>(LocalEntity.Pawn.Address + Offset::Pawn.iIDEntIndex, uHandle) || uHandle == -1)
	{
		angel::_settings->is_aim = false;
	}
	else
	{
		ListEntry = _proc_manager.trace_address(g_game.get_entity_list_address(), { 0x8 * (uHandle >> 9) + 0x10, 0x0 });
		if (ListEntry != 0)
		{
			if (_proc_manager.read_memory<DWORD64>(ListEntry + 0x78 * (uHandle & 0x1FF), PawnAddress))
			{
				if (Entity.UpdatePawn(PawnAddress))
				{
					angel::_settings->is_aim = angel::_settings->crosshair_team_check ? (LocalEntity.Pawn.TeamID != Entity.Pawn.TeamID) : true;
					return;
				}
			}
		}
		angel::_settings->is_aim = false;
	}
}