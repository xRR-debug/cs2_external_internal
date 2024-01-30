#pragma once
#include "../../core/ctx/ctx.h"
#include "../../sdk/classes/entity.h"
#include "../../sdk/classes/bone.h"
#include "../../sdk/globals/globals.hpp"

struct c_utl_vector
{
	DWORD64 Count = 0;
	DWORD64 Data = 0;
};

enum cs_weapon_type : std::uint32_t 
{
	weapon_type_knife = 0,
	weapon_type_pistol = 1,
	weapon_type_submachinegun = 2,
	weapon_type_rifle = 3,
	weapon_type_shotgun = 4,
	weapon_type_sniper_rifle = 5,
	weapon_type_machinegun = 6,
	weapon_type_c4 = 7,
	weapon_type_taser = 8,
	weapon_type_grenade = 9,
	weapon_type_equipment = 10,
	weapon_type_stackable_item = 11,
	weapon_type_fists = 12,
	weapon_type_breach_charge = 13,
	weapon_type_bump_mine = 14,
	weapon_type_tablet = 15,
	weapon_type_melee = 16,
	weapon_type_shield = 17,
	weapon_type_zone_repulsor = 18,
	weapon_type_unknown = 19,
};

class PlayerController
{
public:
	DWORD64 Address = 0;
	int Money = 0;
	int CashSpent = 0;
	int CashSpentTotal = 0;
	int TeamID = 0;
	int Health = 0;
	int AliveStatus = 0;
	DWORD Pawn = 0;
	std::string PlayerName;
public:
	bool GetMoney();
	bool GetTeamID();
	bool GetHealth();
	bool GetIsAlive();
	bool GetPlayerName();
	DWORD64 GetPlayerPawnAddress();
};

class PlayerPawn
{
public:
	enum class Flags
	{
		NONE,
		IN_AIR = 1 << 0
	};

	DWORD64 Address = 0;
	CBone BoneData;
	Vec2 ViewAngle;
	Vec3 Pos;
	Vec2 ScreenPos;
	Vec3 CameraPos;
	Vec3 vec_origin;
	std::string WeaponName;
	DWORD ShotsFired;
	Vec2 AimPunchAngle;
	c_utl_vector AimPunchCache;
	cs_weapon_type weapon_type;
	int Health;
	int Ammo;
	int MaxAmmo;
	int TeamID;
	int Fov;
	DWORD64 bSpottedByMask;
	int fFlags;
	float sensitivity;

public:
	bool GetPos();
	bool GetViewAngle();
	bool GetCameraPos();
	bool GetWeaponName();
	bool GetShotsFired();
	bool GetAimPunchAngle();
	bool GetHealth();
	bool GetTeamID();
	bool GetSensitivity();
	bool GetFov();
	bool GetSpotted();
	bool GetFFlags();
	bool GetAimPunchCache();
	bool GetVecOrigin();
	bool GetAmmo();
	bool GetMaxAmmo();
	bool GetWeaponType();

	constexpr bool HasFlag(const Flags Flag) const noexcept {
		return fFlags & (int)Flag;
	}
};

class CEntity
{
public:
	PlayerController Controller;
	PlayerPawn Pawn;
public:
	bool UpdateController(const DWORD64& PlayerControllerAddress);
	bool UpdatePawn(const DWORD64& PlayerPawnAddress);
	bool UpdateServerPawn(const DWORD64& PlayerPawnAddress);
	bool IsAlive();
	bool IsInScreen();
	CBone GetBone() const;
};