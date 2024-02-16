#include "entity.h"

bool CEntity::UpdateController(const DWORD64& PlayerControllerAddress)
{
	if (PlayerControllerAddress == 0)
		return false;
	this->Controller.Address = PlayerControllerAddress;

	if (!this->Controller.GetHealth())
		return false;
	if (!this->Controller.GetIsAlive())
		return false;
	if (!this->Controller.GetTeamID())
		return false;
	if (!this->Controller.GetPlayerName())
		return false;
	if (!this->Controller.GetMoney())
		return false;

	this->Pawn.Address = this->Controller.GetPlayerPawnAddress();

	return true;
}

bool CEntity::UpdatePawn(const DWORD64& PlayerPawnAddress)
{
	if (PlayerPawnAddress == 0)
		return false;
	this->Pawn.Address = PlayerPawnAddress;

	if (!this->Pawn.GetCameraPos())
		return false;
	if (!this->Pawn.GetPos())
		return false;
	if (!this->Pawn.GetViewAngle())
		return false;
	if (!this->Pawn.GetWeaponName())
		return false;
	if (!this->Pawn.GetAimPunchAngle())
		return false;
	if (!this->Pawn.GetShotsFired())
		return false;
	if (!this->Pawn.GetHealth())
		return false;
	if (!this->Pawn.GetAmmo())
		return false;
	if (!this->Pawn.GetMaxAmmo())
		return false;
	if (!this->Pawn.GetTeamID())
		return false;
	if (!this->Pawn.GetFov())
		return false;
	if (!this->Pawn.GetSpotted())
		return false;
	if (!this->Pawn.GetSensitivity())
		return false;
	if (!this->Pawn.GetFFlags())
		return false;
	if (!this->Pawn.GetAimPunchCache())
		return false;
	if (!this->Pawn.GetWeaponType())
		return false;
	if (!this->Pawn.GetVecOrigin())
		return false;
	if (!this->Pawn.BoneData.UpdateAllBoneData(PlayerPawnAddress))
		return false;

	return true;
}

bool CEntity::UpdateClientData()
{
	if (!this->Client.GetSensitivity())
		return false;

	return true;
}

bool PlayerController::GetMoney()
{
	DWORD64 MoneyServices;
	if (!_proc_manager.read_memory(Address + Offset::InGameMoneyServices.MoneyServices, MoneyServices))
	{
		return false;
	}
	else {
		get_data_address_with_offset<int>(MoneyServices, Offset::InGameMoneyServices.Account, this->Money);
		get_data_address_with_offset<int>(MoneyServices, Offset::InGameMoneyServices.CashSpentThisRound, this->CashSpent);
		get_data_address_with_offset<int>(MoneyServices, Offset::InGameMoneyServices.TotalCashSpent, this->CashSpentTotal);
		return true;
	}
}

bool Client::GetSensitivity()
{
	DWORD64 dwSensitivity;
	float flSensitivity;
	_proc_manager.read_memory(g_game.get_client_dll_address() + Offset::Sensitivity, dwSensitivity);
	if (_proc_manager.read_memory(dwSensitivity + 0x40, flSensitivity))
	{
		this->Sensitivity = flSensitivity;
		return true;
	}
	else
		return false;
}

bool PlayerController::GetTeamID()
{
	return get_data_address_with_offset<int>(Address, Offset::Entity.TeamID, this->TeamID);
}

bool PlayerController::GetHealth()
{
	return get_data_address_with_offset<int>(Address, Offset::Entity.Health, this->Health);
}

bool PlayerController::GetIsAlive()
{
	return get_data_address_with_offset<int>(Address, Offset::Entity.IsAlive, this->AliveStatus);
}

bool PlayerController::GetPlayerName()
{
	char Buffer[MAX_PATH]{};

	if (!_proc_manager.read_memory(Address + Offset::Entity.iszPlayerName, Buffer, MAX_PATH))
		return false;

	this->PlayerName = Buffer;
	if (this->PlayerName.empty())
		this->PlayerName = X("Name_None");

	return true;
}

bool PlayerPawn::GetViewAngle()
{
	return get_data_address_with_offset<Vec2>(Address, Offset::Pawn.angEyeAngles, this->ViewAngle);
}

bool PlayerPawn::GetCameraPos()
{
	return get_data_address_with_offset<Vec3>(Address, Offset::Pawn.vecLastClipCameraPos, this->CameraPos);
}

bool PlayerPawn::GetSpotted()
{
	return get_data_address_with_offset<DWORD64>(Address, Offset::Pawn.bSpottedByMask, this->bSpottedByMask);
}

bool PlayerPawn::GetSensitivity()
{
	return get_data_address_with_offset<float>(Address, Offset::Pawn.m_flMouseSensitivity, this->sensitivity);
}


bool PlayerPawn::GetWeaponName()
{
	DWORD64 WeaponNameAddress = 0;
	char Buffer[525]{};
	
	WeaponNameAddress = _proc_manager.trace_address(this->Address + Offset::Pawn.pClippingWeapon, { 0x10,0x20 ,0x0 });
	if (WeaponNameAddress == 0)
		return false;

	if (!_proc_manager.read_memory(WeaponNameAddress, Buffer, 525))
		return false;

	WeaponName = std::string(Buffer);
	std::size_t Index = WeaponName.find("_");
	if (Index == std::string::npos || WeaponName.empty())
	{
		WeaponName = X("Weapon_None");
	}
	else
	{
		WeaponName = WeaponName.substr(Index + 1, WeaponName.size() - Index - 1);
	}

	return true;
}

bool PlayerPawn::GetShotsFired()
{
	return get_data_address_with_offset<DWORD>(Address, Offset::Pawn.iShotsFired, this->ShotsFired);
}

bool PlayerPawn::GetAimPunchAngle()
{
	return get_data_address_with_offset<Vec2>(Address, Offset::Pawn.aimPunchAngle, this->AimPunchAngle);
}

bool PlayerPawn::GetTeamID()
{
	return get_data_address_with_offset<int>(Address, Offset::Pawn.iTeamNum, this->TeamID);
}

bool PlayerPawn::GetAimPunchCache()
{
	return get_data_address_with_offset<c_utl_vector>(Address, Offset::Pawn.aimPunchCache, this->AimPunchCache);
}

bool PlayerPawn::GetVecOrigin() {
	DWORD64 game_scene_node = 0;

	if (!_proc_manager.read_memory<DWORD64>(Address + Offset::Pawn.GameSceneNode, game_scene_node)) {
		return false;
	}

	return get_data_address_with_offset<Vec3>(game_scene_node, Offset::Pawn.vec_origin, this->vec_origin);
}

DWORD64 PlayerController::GetPlayerPawnAddress()
{
	DWORD64 EntityPawnListEntry = 0;
	DWORD64 EntityPawnAddress = 0;

	if (!get_data_address_with_offset<DWORD>(Address, Offset::Entity.PlayerPawn, this->Pawn))
		return 0;

	if (!_proc_manager.read_memory<DWORD64>(g_game.get_entity_list_address(), EntityPawnListEntry))
		return 0;

	if (!_proc_manager.read_memory<DWORD64>(EntityPawnListEntry + 0x10 + 8 * ((Pawn & 0x7FFF) >> 9), EntityPawnListEntry))
		return 0;

	if (!_proc_manager.read_memory<DWORD64>(EntityPawnListEntry + 0x78 * (Pawn & 0x1FF), EntityPawnAddress))
		return 0;

	return EntityPawnAddress;
}

bool PlayerPawn::GetPos()
{
	return get_data_address_with_offset<Vec3>(Address, Offset::Pawn.Pos, this->Pos);
}

bool PlayerPawn::GetHealth()
{
	return get_data_address_with_offset<int>(Address, Offset::Pawn.CurrentHealth, this->Health);
}

bool PlayerPawn::GetAmmo()
{
	DWORD64 ClippingWeapon = 0;
	if (!_proc_manager.read_memory<DWORD64>(Address + Offset::WeaponBaseData.ClippingWeapon, ClippingWeapon))
		return false;

	return get_data_address_with_offset<int>(ClippingWeapon, Offset::WeaponBaseData.Clip1, this->Ammo);
}

bool PlayerPawn::GetMaxAmmo()
{
	DWORD64 ClippingWeapon = 0;
	DWORD64 WeaponData = 0;
	if (!_proc_manager.read_memory<DWORD64>(Address + Offset::WeaponBaseData.ClippingWeapon, ClippingWeapon))
		return false;
	if (!_proc_manager.read_memory<DWORD64>(ClippingWeapon + Offset::WeaponBaseData.WeaponDataPTR, WeaponData))
		return false;

	return get_data_address_with_offset<int>(WeaponData, Offset::WeaponBaseData.MaxClip, this->MaxAmmo);
}

bool PlayerPawn::GetWeaponType() 
{
	DWORD64 clipping_weapon = 0;
	DWORD64 weapon_data = 0;

	if (!_proc_manager.read_memory<DWORD64>(Address + Offset::WeaponBaseData.ClippingWeapon, clipping_weapon)) 
	{
		return false;
	}

	if (!_proc_manager.read_memory<DWORD64>(clipping_weapon + Offset::WeaponBaseData.WeaponDataPTR, weapon_data)) 
	{
		return false;
	}

	return get_data_address_with_offset<cs_weapon_type>(weapon_data, Offset::WeaponBaseData.WeaponType, this->weapon_type);
}

bool PlayerPawn::GetFov()
{
	DWORD64 CameraServices = 0;
	if (!_proc_manager.read_memory<DWORD64>(Address + Offset::Pawn.CameraServices, CameraServices))
		return false;
	return get_data_address_with_offset<int>(CameraServices, Offset::Pawn.iFovStart, this->Fov);
}

bool PlayerPawn::GetFFlags()
{
	return get_data_address_with_offset<int>(Address, Offset::Pawn.fFlags, this->fFlags);
}

bool CEntity::IsAlive()
{
	return this->Controller.AliveStatus == 1 && this->Pawn.Health > 0;
}

bool CEntity::IsInScreen()
{
	return g_game.view.WorldToScreen(this->Pawn.Pos, this->Pawn.ScreenPos);
}

CBone CEntity::GetBone() const
{
	if (this->Pawn.Address == 0)
		return CBone{};
	return this->Pawn.BoneData;
}
