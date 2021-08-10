#pragma once
#include "pch.h"

class UHealthStatsComponent
{
public:
	char pad_0000[192]; //0x0000
	int32_t Health; //0x00C0
	float Stamina; //0x00C4
	char pad_00C8[184]; //0x00C8
}; //Size: 0x0180
static_assert(sizeof(UHealthStatsComponent) == 0x180);

class AItem_Weapon_General
{
public:
	char pad_0000[712]; //0x0000
	int32_t N000000A8; //0x02C8
	int32_t WeaponDamage; //0x02CC
	char pad_02D0[8]; //0x02D0
	int32_t MaxMagazineAmmo; //0x02D8
	int32_t MaxStockAmmo; //0x02DC
	float TimeBetweenShots; //0x02E0
	float WeaponRecoil; //0x02E4
	float WeaponRecoilAlphaPerShot; //0x02E8
	float AccuracyHip; //0x02EC
	float AccuracySight; //0x02F0
	float SpreadShot; //0x02F4
	char pad_02F8[176]; //0x02F8
	int32_t CurrentMagazineAmmo; //0x03A8
	uint16_t CurrentStockAmmo; //0x03AC
	char pad_03AE[2]; //0x03AE
	float CurrentSpread; //0x03B0
	char pad_03B4[460]; //0x03B4
}; //Size: 0x0580
static_assert(sizeof(AItem_Weapon_General) == 0x580);