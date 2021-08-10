#pragma once
#include "pch.h"

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
	std::vector<uintptr_t> m_offsets;
public:
	Anticheat(std::vector<uintptr_t>offsets);
public:
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
    void Nop(BYTE* dst, unsigned int size);
	void DisableScue4x64();
	void DisableCEDetection();
};
