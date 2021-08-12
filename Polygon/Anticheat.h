#pragma once
#include "pch.h"
#include "Memory.h"

class Anticheat
{
public:
	enum Scue4x64
	{
		CreateProcessW,
		GetProcessId,
		FindWindowA,
	};
private:
	NopInternal*   DisableCreateProcessW;
	PatchInternal* DisableGetProcId;
	PatchInternal* DisableFindWindowA;
private:
	std::vector<uintptr_t> m_offsets;
public:
	Anticheat(std::vector<uintptr_t>offsets);
	~Anticheat();
public:
	void DisableScue4x64();
	void DisableCEDetection();
};
