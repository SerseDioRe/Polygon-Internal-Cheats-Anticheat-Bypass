#pragma once
#include "functions.h"

class Signatures
{
public:
	enum ESignatures
	{
		AItem_Weapon_General = 3,
		UHealthStatsComponent = 4,
		UnlimitedStaminaOne = 5,
		UnlimitedStaminaTwo = 6,
	};
private:
	const char* m_gameName;
private:
	uintptr_t m_addressCreateProcessW;
	uintptr_t m_addressGetProcId;
	uintptr_t m_addressFindWindowA;
	uintptr_t m_addressGetAItem_Weapon_General;
	uintptr_t m_addressGetUHealthStatsComponent;
	uintptr_t m_unlimitedStaminaOne;
	uintptr_t m_unlimitedStaminaTwo;
public:
	Signatures(const char* gameName);
	std::vector<uintptr_t> GetOffsets();
	bool checkIfIsValid();
};
