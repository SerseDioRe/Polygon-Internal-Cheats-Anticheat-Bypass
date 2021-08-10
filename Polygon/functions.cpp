#include "functions.h"

namespace Functions
{
    PEB* GetPEB()
    {
        PEB* peb = (PEB*)__readgsqword(0x60); //64bit

        return peb;
    }

    _LDR_DATA_TABLE_ENTRY GetLDREntryIn(TCHAR* modName)
    {
        _LDR_DATA_TABLE_ENTRY ldr{ 0 };

        PEB* peb = GetPEB();

        LIST_ENTRY head = peb->Ldr->InMemoryOrderModuleList;
        LIST_ENTRY curr = head;

        while (curr.Flink != head.Blink)
        {
            _LDR_DATA_TABLE_ENTRY* mod = (_LDR_DATA_TABLE_ENTRY*)CONTAINING_RECORD(curr.Flink, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

            if (mod->BaseDllName.Buffer)
            {
                if (_tcsicmp(mod->BaseDllName.Buffer, modName) == 0)
                {
                    ldr = *mod;
                    return ldr;
                }
            }
            curr = *curr.Flink;
        }
        return ldr;
    }

    uintptr_t _GetModuleHandleIn(TCHAR* modName)
    {
        _LDR_DATA_TABLE_ENTRY mod = GetLDREntryIn(modName);

        return (uintptr_t)mod.DllBase;
    }

    char* ScanBasic(char* pattern, char* mask, char* begin, intptr_t size)
    {
        intptr_t patternLen = strlen(mask);

        for (int i = 0; i < size; i++)
        {
            bool found = true;
            for (int j = 0; j < patternLen; j++)
            {
                if (mask[j] != '?' && pattern[j] != *(char*)((intptr_t)begin + i + j))
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                return (begin + i);
            }
        }
        return nullptr;
    }

    char* ScanInternal(char* pattern, char* mask, char* begin, intptr_t size)
    {
        char* match{ nullptr };
        MEMORY_BASIC_INFORMATION mbi{};

        for (char* curr = begin; curr < begin + size; curr += mbi.RegionSize)
        {
            if (!VirtualQuery(curr, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS) continue;

            match = ScanBasic(pattern, mask, curr, mbi.RegionSize);

            if (match != nullptr)
            {
                break;
            }
        }
        return match;
    }

    char* TO_CHAR(wchar_t* string)
    {
        size_t len = wcslen(string) + 1;
        char* c_string = new char[len];
        size_t numCharsRead;
        wcstombs_s(&numCharsRead, c_string, len, string, _TRUNCATE);
        return c_string;
    }

    LDR_DATA_TABLE_ENTRY* GetLDREntry(std::string name)
    {
        LDR_DATA_TABLE_ENTRY* ldr = nullptr;

        PEB* peb = GetPEB();

        LIST_ENTRY head = peb->Ldr->InMemoryOrderModuleList;

        LIST_ENTRY curr = head;

        while (curr.Flink != head.Blink)
        {
            LDR_DATA_TABLE_ENTRY* mod = (LDR_DATA_TABLE_ENTRY*)CONTAINING_RECORD(curr.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

            if (mod->FullDllName.Buffer)
            {
                char* cName = TO_CHAR(mod->BaseDllName.Buffer);

                if (_stricmp(cName, name.c_str()) == 0)
                {
                    ldr = mod;
                    break;
                }
                delete[] cName;
            }
            curr = *curr.Flink;
        }
        return ldr;
    }

    char* ScanModIn(char* pattern, char* mask, std::string modName)
    {
        LDR_DATA_TABLE_ENTRY* ldr = GetLDREntry(modName);

        char* match = ScanInternal(pattern, mask, (char*)ldr->DllBase, ldr->SizeOfImage);

        return match;
    }

    LDR_DATA_TABLE_ENTRY* GetLDREntryInternal(const wchar_t* modName)
    {
        LDR_DATA_TABLE_ENTRY* modEntry = nullptr;

        PEB* peb = GetPEB();

        LIST_ENTRY head = peb->Ldr->InMemoryOrderModuleList;

        LIST_ENTRY curr = head;

        for (auto curr = head; curr.Flink != &peb->Ldr->InMemoryOrderModuleList; curr = *curr.Flink)
        {
            LDR_DATA_TABLE_ENTRY* mod = (LDR_DATA_TABLE_ENTRY*)CONTAINING_RECORD(curr.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

            if (mod->BaseDllName.Buffer)
            {
                if (_wcsicmp(modName, mod->BaseDllName.Buffer) == 0)
                {
                    modEntry = mod;
                    break;
                }
            }
        }
        return modEntry;
    }

    char* GetModuleBaseAddressInternalPEB(const wchar_t* modName)
    {
        LDR_DATA_TABLE_ENTRY* modEntry = GetLDREntryInternal(modName);

        return (char*)modEntry->DllBase;
    }

    uintptr_t GetOffset(uintptr_t addr, int off)
    {
        uintptr_t address = 0x0;
        address = (addr + *(uint32_t*)addr + off) - (uintptr_t)GetModuleBaseAddressInternalPEB(L"gta-core-five.dll");
        return address;
    }

    uintptr_t GetOffsetNet(uintptr_t addr, int off)
    {
        uintptr_t address = 0x0;
        address = (addr + *(uint32_t*)addr + off) - (uintptr_t)GetModuleBaseAddressInternalPEB(L"net.dll");
        return address;
    }

    LPCWSTR getStringLpcwstrG(const std::string& str)
    {
        std::wstring dllWstring = std::wstring(str.begin(), str.end());
        LPCWSTR      dllLpcwstr = dllWstring.c_str();

        return dllLpcwstr;
    }
}