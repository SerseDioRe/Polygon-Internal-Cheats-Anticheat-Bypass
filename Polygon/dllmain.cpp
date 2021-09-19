// dllmain.cpp : Definisce il punto di ingresso per l'applicazione DLL.
#include "pch.h"
#include "functions.h"
#include "Anticheat.h"
#include "Polygon.h"
#include "Signatures.h"
#include "Memory.h"

#include <TlHelp32.h>
#include <Psapi.h>

//detours
#include "detours.h"
#if defined _M_X64
#pragma comment(lib, "detours.X64/detours.lib")
#elif defined _M_IX86
#pragma comment(lib, "detours.X86/detours.lib")
#endif
//

#define UNLIMITED_AMMO_KEY_PRESSED GetAsyncKeyState(VK_F1) & 1
#define NO_RECOIL_KEY_PRESSED GetAsyncKeyState(VK_F2) & 1
#define RAPID_FIRE_KEY_PRESSED GetAsyncKeyState(VK_F3) & 1
#define UNLIMITED_STAMINA_KEY_PRESSED GetAsyncKeyState(VK_F4) & 1

#define RVA(addr, size) ((uintptr_t)((UINT_PTR)(addr) + *(PINT)((UINT_PTR)(addr) + ((size) - sizeof(INT))) + (size)))

//ANTICHEAT
uintptr_t addressSHValue     = 0;
uintptr_t addressGetProcId   = 0;
uintptr_t addressFindWindowA = 0;
//HACK
uintptr_t addressGetAItem_Weapon_General = 0;
uintptr_t addressAItem_Weapon_General    = 0;
uintptr_t addressUHealthStatsComponent   = 0;
uintptr_t addressACharacter              = 0;

Anticheat* anticheat                         = nullptr;
Signatures* signatures                       = nullptr;
AItem_Weapon_General* aItem_Weapon_General   = nullptr;
UHealthStatsComponent* uHealthStatsComponent = nullptr;
GWorld* gWorld = nullptr;

NopInternal* UnlimitedStaminaWhenRun  = nullptr;
NopInternal* UnlimitedStaminaWhenJump = nullptr;

bool bRecoil;
bool unlimitedAmmo;
bool unlimitedStamina;
bool bRapidFire;

uintptr_t gWorldAddress = 0;
uintptr_t gWorldOffset = 0;

MODULEINFO GetModuleInfo(char* szModule)
{
    MODULEINFO modInfo = { 0 };
    HMODULE hModule = GetModuleHandleA(szModule);
    if (hModule != 0)
        GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
    return modInfo;
}

uintptr_t FindPattern(char* module, char* pattern, char* mask)
{
    MODULEINFO mInfo = GetModuleInfo(module);
    uintptr_t base = (uintptr_t)mInfo.lpBaseOfDll;
    DWORD size = (DWORD)mInfo.SizeOfImage;
    DWORD patternLength = (DWORD)strlen(mask);

    for (DWORD i = 0; i < size - patternLength; i++)
    {
        bool found = true;
        for (DWORD j = 0; j < patternLength; j++)
        {
            found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
        }
        if (found)
            return base + i;
    }
    return NULL;
}

uintptr_t GetOffset(uintptr_t addr)
{
    uintptr_t address = 0x0;
    address = (addr + *(uint32_t*)addr + 4) - (uintptr_t)GetModuleHandle(NULL);
    return address;
}

typedef __int64(__fastcall* tGetAItem_Weapon_General)(uintptr_t AItem_Weapon_GeneralObj);
tGetAItem_Weapon_General GetAItem_Weapon_GeneralObj = nullptr;

__int64 __fastcall HookGetAItem_Weapon_GeneralObj(uintptr_t AItem_Weapon_GeneralObj)
{
    addressAItem_Weapon_General = AItem_Weapon_GeneralObj;
    return GetAItem_Weapon_GeneralObj(AItem_Weapon_GeneralObj);
}

typedef void(__fastcall* tGetUHealthStatsComponentObj)(uintptr_t UHealthStatsComponentObj);
tGetUHealthStatsComponentObj GetUHealthStatsComponentObj = nullptr;

void __fastcall HookGetUHealthStatsComponentObj(uintptr_t UHealthStatsComponentObj)
{
    addressUHealthStatsComponent = UHealthStatsComponentObj;
    GetUHealthStatsComponentObj(UHealthStatsComponentObj);
}

DWORD WINAPI PolygonHack(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    gWorldAddress = sigscanporcodio("POLYGON-Win64-Shipping.exe", "48 8B 1D ?? ?? ?? ?? 48 85 DB 74 ?? 41 B0 ??");
    //gWorldAddress = FindPattern((char*)"POLYGON-Win64-Shipping.exe", (char*)"\x48\x8b\x1d\x00\x00\x00\x00\x48\x85\xdb\x74\x00\x41\xb0", (char*)"xxx????xxxx?xx");
    gWorldOffset = RVA(gWorldAddress, 7);//GetOffset(gWorldAddress);

    /*signatures = new Signatures("POLYGON-Win64-Shipping.exe");

    std::vector<uintptr_t> Offsets{ signatures->GetOffsets() };
    if (!signatures->checkIfIsValid())
        return NULL;

    anticheat = new Anticheat({ Offsets[Anticheat::CreateProcessW], Offsets[Anticheat::GetProcessId],
        Offsets[Anticheat::FindWindowA] });

    anticheat->DisableScue4x64();
    anticheat->DisableCEDetection();

    //STARTING HACK...
    GetAItem_Weapon_GeneralObj  = (tGetAItem_Weapon_General)(Offsets[Signatures::AItem_Weapon_General]);
    GetUHealthStatsComponentObj = (tGetUHealthStatsComponentObj)(Offsets[Signatures::UHealthStatsComponent]);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)GetAItem_Weapon_GeneralObj, (PBYTE)HookGetAItem_Weapon_GeneralObj);
    DetourAttach(&(LPVOID&)GetUHealthStatsComponentObj, (PBYTE)HookGetUHealthStatsComponentObj);
    DetourTransactionCommit();

    UnlimitedStaminaWhenRun = new NopInternal((BYTE*)(Offsets[Signatures::UnlimitedStaminaOne]), 8);
    UnlimitedStaminaWhenJump = new NopInternal((BYTE*)(Offsets[Signatures::UnlimitedStaminaTwo]), 8);*/

	/*while (true)
	{
        gWorld = (GWorld*)((uintptr_t)GetModuleHandle(NULL) + 0x5A79370); // 48 8B 1D ? ? ? ? 48 85 DB 74 ? 41 B0 ?
        aItem_Weapon_General = (AItem_Weapon_General*)(gWorld->GameWorld->OwningGameInstance->LocalPlayers->LocalPlayer->PlayerController->AcknowledgedPawn->WeaponComponent->CurrentWeapon);
        uHealthStatsComponent = (UHealthStatsComponent*)(gWorld->GameWorld->OwningGameInstance->LocalPlayers->LocalPlayer->PlayerController->AcknowledgedPawn->HealthStatsComponent);

        std::cout << std::hex << std::uppercase << "0x" << addressACharacter << '\n';

        if(UNLIMITED_AMMO_KEY_PRESSED)
        {
            if (aItem_Weapon_General)
                unlimitedAmmo = !unlimitedAmmo;
        }

        if(NO_RECOIL_KEY_PRESSED)
        {
            if(aItem_Weapon_General)
                bRecoil = !bRecoil;
        }

        if(RAPID_FIRE_KEY_PRESSED)
            bRapidFire = !bRapidFire;

        if(UNLIMITED_STAMINA_KEY_PRESSED)
        {
            if (uHealthStatsComponent)
                unlimitedStamina = !unlimitedStamina;

            if (unlimitedStamina)
            {
                if (uHealthStatsComponent)
                {
                    UnlimitedStaminaWhenRun->enable();
                    UnlimitedStaminaWhenJump->enable();
                }
            }
            else
            {
                UnlimitedStaminaWhenRun->disable();
                UnlimitedStaminaWhenJump->disable();
            }
        }

        if (bRecoil)
        {
            if (aItem_Weapon_General)
            {
                aItem_Weapon_General->WeaponRecoil = 0.0F;
                aItem_Weapon_General->WeaponRecoilAlphaPerShot = 0.0F;
                aItem_Weapon_General->SpreadShot = 0.0F;
                aItem_Weapon_General->AccuracyHip = 0.0F;
                aItem_Weapon_General->AccuracySight = 0.0F;
                aItem_Weapon_General->CurrentSpread = 0.0F;
            }
        }

        if (unlimitedAmmo)
        {
            if (aItem_Weapon_General)
            {
                aItem_Weapon_General->CurrentMagazineAmmo = 330;
                aItem_Weapon_General->CurrentStockAmmo = 330;
            }
        }

        if(bRapidFire)
        {
            if(aItem_Weapon_General)
                aItem_Weapon_General->TimeBetweenShots = 0.01F;
        }

		Sleep(30);
	}*/

    //delete anticheat;
    //delete signatures;
    //delete aItem_Weapon_General;
    //delete uHealthStatsComponent;
    //delete UnlimitedStaminaWhenRun;
    //delete UnlimitedStaminaWhenJump;
    //fclose(f);
    //FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);

	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)PolygonHack, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

