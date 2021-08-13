#pragma once
#include "pch.h"

enum class EWeaponShootingType {
	BOLT = 0,
	SEMI_AUTO = 1,
	AUTO = 2,
	EWeaponShootingType_MAX = 3
};

enum class EWeaponType {
	NONE = 0,
	RIFLE = 1,
	SNIPER = 2,
	PISTOL = 3,
	EWeaponType_MAX = 4
};

class GWorld
{
public:
	class UWorld* GameWorld; //0x0000
	char pad_0008[120]; //0x0008
}; //Size: 0x0080
static_assert(sizeof(GWorld) == 0x80);

class UWorld
{
public:
	char pad_0000[384]; //0x0000
	class UGameInstance* OwningGameInstance; //0x0180
}; //Size: 0x0188
static_assert(sizeof(UWorld) == 0x188);

class UGameInstance
{
public:
	char pad_0000[56]; //0x0000
	class ULocalPlayer* LocalPlayers; //0x0038
	char pad_0040[72]; //0x0040
}; //Size: 0x0088
static_assert(sizeof(UGameInstance) == 0x88);

class ULocalPlayer
{
public:
	class UULocalPlayer* LocalPlayer; //0x0000
	char pad_0008[128]; //0x0008
}; //Size: 0x0088
static_assert(sizeof(ULocalPlayer) == 0x88);

class UULocalPlayer
{
public:
	char pad_0000[48]; //0x0000
	class APlayerController* PlayerController; //0x0030
	char pad_0038[80]; //0x0038
}; //Size: 0x0088
static_assert(sizeof(UULocalPlayer) == 0x88);

class APlayerController
{
public:
	char pad_0000[672]; //0x0000
	class APawn* AcknowledgedPawn; //0x02A0
	char pad_02A8[32]; //0x02A8
}; //Size: 0x02C8
static_assert(sizeof(APlayerController) == 0x2C8);

class APawn
{
public:
	char pad_0000[1224]; //0x0000
	int8_t N000000E8; //0x04C8
	int8_t PlayerAction; //0x04C9
	char pad_04CA[14]; //0x04CA
	int32_t N000000EA; //0x04D8
	int8_t CameraViewMode; //0x04DC
	char pad_04DD[27]; //0x04DD
	class AActor* LastFocusActor; //0x04F8
	class AActor* CurrentInteractActor; //0x0500
	char pad_0508[80]; //0x0508
	class UCameraComponent* FirstPersonCamera; //0x0558
	class USpringArmComponent* ThirdPersonCameraBoom; //0x0560
	class UCameraComponent* ThirdPersonCamera; //0x0568
	class UInputComponent* PlayerInputComponent; //0x0570
	class UWidgetComponent* WidgetPlayerMarker; //0x0578
	class UHealthStatsComponent* HealthStatsComponent; //0x0580
	class UWeaponComponent* WeaponComponent; //0x0588
}; //Size: 0x0590
static_assert(sizeof(APawn) == 0x590);

class UHealthStatsComponent
{
public:
	char pad_0000[192]; //0x0000
	int32_t Health; //0x00C0
	float Stamina; //0x00C4
	char pad_00C8[184]; //0x00C8
}; //Size: 0x0180
static_assert(sizeof(UHealthStatsComponent) == 0x180);

class UWeaponComponent
{
public:
	char pad_0000[280]; //0x0000
	class AItem_Weapon_General* CurrentWeapon; //0x0118
	class AItem_Weapon_General* PrimaryWeapon; //0x0120
	class AItem_Weapon_General* SecondaryWeapon; //0x0128
	char pad_0130[88]; //0x0130
}; //Size: 0x0188
static_assert(sizeof(UWeaponComponent) == 0x188);

class AActor
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(AActor) == 0x88);

class USpringArmComponent
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(USpringArmComponent) == 0x88);

class UCameraComponent
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(UCameraComponent) == 0x88);

class UInputComponent
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(UInputComponent) == 0x88);

class UWidgetComponent
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(UWidgetComponent) == 0x88);

class AItem_Weapon_General
{
public:
	char pad_0000[712]; //0x0000
	int8_t WeaponType; //0x02C8
	int8_t WeaponShootingType; //0x02C9
	char pad_02CA[2]; //0x02CA
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
	char pad_02F8[24]; //0x02F8
	int32_t LevelRequired; //0x0310
	bool bIsPremium; //0x0314
	bool bIsAvailable; //0x0315
	char pad_0316[146]; //0x0316
	int32_t CurrentMagazineAmmo; //0x03A8
	uint16_t CurrentStockAmmo; //0x03AC
	char pad_03AE[2]; //0x03AE
	float CurrentSpread; //0x03B0
	char pad_03B4[460]; //0x03B4
}; //Size: 0x0580
static_assert(sizeof(AItem_Weapon_General) == 0x580);