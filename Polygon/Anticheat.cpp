#include "Anticheat.h"

Anticheat::Anticheat(std::vector<uintptr_t>offsets) :m_offsets{ offsets } {}

void Anticheat::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
    DWORD oldprotect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

    memcpy(dst, src, size);
    VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void Anticheat::Nop(BYTE* dst, unsigned int size)
{
    DWORD oldprotect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
    memset(dst, 0x90, size);
    VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void Anticheat::DisableScue4x64()
{
    Nop((BYTE*)m_offsets[Anticheat::CreateProcessW], 6);
}

void Anticheat::DisableCEDetection()
{
    Patch((BYTE*)m_offsets[Anticheat::GetProcessId], (BYTE*)"\xC3\x90", 2);
    Patch((BYTE*)m_offsets[Anticheat::FindWindowA],  (BYTE*)"\xC3\x90", 2);
}