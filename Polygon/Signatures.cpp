#include "Signatures.h"

Signatures::Signatures(const char* gameName) :m_gameName{ gameName } {}

std::vector<uintptr_t> Signatures::GetOffsets()
{
    m_addressCreateProcessW = (uintptr_t)(Functions::ScanModIn((char*)"\xFF\x15\x00\x00\x00\x00\x85\xC0\x0F\x85\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x66\x44\x89\x7D\x70",
        (char*)"xx????xxxx????xx????xxxxx",
        m_gameName));

    m_addressGetProcId = (uintptr_t)(Functions::ScanModIn((char*)"\x40\x56\x41\x56\x48\x81\xEC\x88\x02\x00\x00\x48\x8B\x00\x00\x00\x00\x00\x48\x33\xC4",
        (char*)"xxxxxxxxxxxxx?????xxx",
        m_gameName));

    m_addressFindWindowA = (uintptr_t)(Functions::ScanModIn((char*)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x83\xC1\x30\x48\x83\x39\x00\x74\x09\xE8\x00\x00\x00\x00\x84\xC0\x75\x08",
        (char*)"xxxxxxxxxxxxxxxxxxxx????xxxx",
        m_gameName));

    m_addressGetAItem_Weapon_General = (uintptr_t)(Functions::ScanModIn((char*)"\x48\x8B\xC4\x48\x89\x58\x10\x48\x89\x70\x18\x48\x89\x78\x20\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x68\xA1\x48\x81\xEC\xF0\x00\x00\x00\x0F\x29\x70\xC8\x0F\x29\x78\xB8\x48\x8B\x00\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x07\x48\x8B\xD9",
        (char*)"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?????xxxxxxxxxx",
        m_gameName));

    m_addressGetUHealthStatsComponent = (uintptr_t)(Functions::ScanModIn((char*)"\x40\x53\x48\x83\xEC\x30\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\x83\xA0\x00\x00\x00\x48\x85\xC0\x0F\x84",
        (char*)"xxxxxxxxxx????xxxxxxxxxxxx",
        m_gameName));

    m_unlimitedStaminaOne = (uintptr_t)(Functions::ScanModIn((char*)"\xF3\x0F\x11\x83\xC4\x00\x00\x00\x0F\x87\x00\x00\x00\x00\x48\x8B\xCB\xC6\x83\xC9\x00\x00\x00\x00\xE8",
        (char*)"xxxxxxxxxx????xxxxxxxxxxx",
        m_gameName));

    m_unlimitedStaminaTwo = (uintptr_t)(Functions::ScanModIn((char*)"\xF3\x0F\x11\x81\xC4\x00\x00\x00\x48\x8B\x8B\x80\x05\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x7C\x24\x30",
        (char*)"xxxxxxxxxxxxxxxx????xxxxx",
        m_gameName));

    return 
    { 
        m_addressCreateProcessW, m_addressGetProcId, m_addressFindWindowA, 
        m_addressGetAItem_Weapon_General, m_addressGetUHealthStatsComponent,
        m_unlimitedStaminaOne, m_unlimitedStaminaTwo 
    };
}

bool Signatures::checkIfIsValid()
{
    if (m_addressCreateProcessW == 0 || m_addressGetProcId == 0 ||
        m_addressFindWindowA == 0 || m_addressGetAItem_Weapon_General == 0)
    {
        MessageBox(0, L"Signature seem broken, Game needs to be updated", L"ERROR", MB_ICONERROR);
        return false;
    }
    else
        return true;

}