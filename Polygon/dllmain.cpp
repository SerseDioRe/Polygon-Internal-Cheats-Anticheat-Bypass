// dllmain.cpp : Definisce il punto di ingresso per l'applicazione DLL.
#include "pch.h"
#include "functions.h"
#include "Anticheat.h"
#include "Weapon.h"

//detours
#include "detours.h"
#if defined _M_X64
#pragma comment(lib, "detours.X64/detours.lib")
#elif defined _M_IX86
#pragma comment(lib, "detours.X86/detours.lib")
#endif

//ANTICHEAT
uintptr_t addressSHValue     = 0;
uintptr_t addressGetProcId   = 0;
uintptr_t addressFindWindowA = 0;
//HACK
uintptr_t addressGetAItem_Weapon_General = 0;
uintptr_t addressAItem_Weapon_General = 0;

Anticheat* anticheat = nullptr;
AItem_Weapon_General* aItem_Weapon_General = nullptr;

bool bRecoil;
bool unlimitedAmmo;
bool unlimitedStamina;

typedef __int64(__fastcall* tGetAItem_Weapon_General)(uintptr_t AItem_Weapon_GeneralObj);
tGetAItem_Weapon_General GetAItem_Weapon_GeneralObj = nullptr;

__int64 __fastcall HookGetAItem_Weapon_GeneralObj(uintptr_t AItem_Weapon_GeneralObj)
{
    addressAItem_Weapon_General = AItem_Weapon_GeneralObj;
    return GetAItem_Weapon_GeneralObj(AItem_Weapon_GeneralObj);
}

DWORD WINAPI PolygonHack(HMODULE hModule)
{
    const char* gameName = "POLYGON-Win64-Shipping.exe";

    uintptr_t moduleBase = (uintptr_t)Functions::_GetModuleHandleIn((_TCHAR*)(gameName));

    addressSHValue       = (uintptr_t)(Functions::ScanModIn((char*)"\xFF\x15\x00\x00\x00\x00\x85\xC0\x0F\x85\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x66\x44\x89\x7D\x70", 
        (char*)"xx????xxxx????xx????xxxxx", 
        gameName));
    addressGetProcId     = (uintptr_t)(Functions::ScanModIn((char*)"\x40\x56\x41\x56\x48\x81\xEC\x88\x02\x00\x00\x48\x8B\x00\x00\x00\x00\x00\x48\x33\xC4",
        (char*)"xxxxxxxxxxxxx?????xxx",
        gameName));
    addressFindWindowA   = (uintptr_t)(Functions::ScanModIn((char*)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x83\xC1\x30\x48\x83\x39\x00\x74\x09\xE8\x00\x00\x00\x00\x84\xC0\x75\x08",
        (char*)"xxxxxxxxxxxxxxxxxxxx????xxxx",
        gameName));

    addressGetAItem_Weapon_General = (uintptr_t)(Functions::ScanModIn((char*)"\x48\x8B\xC4\x48\x89\x58\x10\x48\x89\x70\x18\x48\x89\x78\x20\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x68\xA1\x48\x81\xEC\xF0\x00\x00\x00\x0F\x29\x70\xC8\x0F\x29\x78\xB8\x48\x8B\x00\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x07\x48\x8B\xD9",
        (char*)"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?????xxxxxxxxxx",
        gameName));

    if (addressSHValue == 0 || addressGetProcId == 0 || 
        addressFindWindowA == 0 || addressGetAItem_Weapon_General == 0)
    {
        MessageBox(0, L"Signature seem broken, Game needs to be updated", L"ERROR", MB_ICONERROR);
        return NULL;
    }

    std::vector<uintptr_t> anticheatOffsets{ addressSHValue, addressGetProcId, addressFindWindowA };
    anticheat = new Anticheat(anticheatOffsets);

    anticheat->DisableScue4x64();
    anticheat->DisableCEDetection();

    //STARTING HACK...
    GetAItem_Weapon_GeneralObj = (tGetAItem_Weapon_General)(addressGetAItem_Weapon_General);//addressGetAItem_Weapon_General);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)GetAItem_Weapon_GeneralObj, (PBYTE)HookGetAItem_Weapon_GeneralObj);
    DetourTransactionCommit();

	while (true) 
	{
        // Hack...
        aItem_Weapon_General = (AItem_Weapon_General*)(addressAItem_Weapon_General);

        if (GetAsyncKeyState(VK_NUMPAD1) & 1) 
        {
            if(aItem_Weapon_General)
            {
                bRecoil = !bRecoil;
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
                aItem_Weapon_General->TimeBetweenShots = 0.1F;
            }
        }

        if(GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            if(aItem_Weapon_General)
            {
                unlimitedAmmo = !unlimitedAmmo;
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

		Sleep(30);
	}

    delete anticheat;

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

