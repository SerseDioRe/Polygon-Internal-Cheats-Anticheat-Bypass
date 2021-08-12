#include "Anticheat.h"

Anticheat::Anticheat(std::vector<uintptr_t>offsets) :m_offsets{ offsets } 
{
    DisableCreateProcessW = new NopInternal(  (BYTE*)m_offsets[Anticheat::CreateProcessW], 6);
    DisableGetProcId      = new PatchInternal((BYTE*)m_offsets[Anticheat::GetProcessId], (BYTE*)"\xC3\x90", 2);
    DisableFindWindowA    = new PatchInternal((BYTE*)m_offsets[Anticheat::FindWindowA],  (BYTE*)"\xC3\x90", 2);
}

Anticheat::~Anticheat()
{
    DisableCreateProcessW->disable();
    DisableGetProcId->disable();
    DisableFindWindowA->disable();
    delete DisableCreateProcessW;
    delete DisableGetProcId;
    delete DisableFindWindowA;
}

void Anticheat::DisableScue4x64()
{
    DisableCreateProcessW->enable();
}

void Anticheat::DisableCEDetection()
{
    DisableGetProcId->enable();
    DisableFindWindowA->enable();
}