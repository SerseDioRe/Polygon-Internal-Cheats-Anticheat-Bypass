#pragma once
#include "pch.h"
#include "ntapi.h"

namespace Functions
{
    PEB*                  GetPEB();
    _LDR_DATA_TABLE_ENTRY GetLDREntryIn                   (TCHAR* modName);
    uintptr_t             _GetModuleHandleIn              (TCHAR* modName);
    char*                 ScanBasic                       (char* pattern, char* mask, char* begin, intptr_t size);
    char*                 ScanInternal                    (char* pattern, char* mask, char* begin, intptr_t size);
    char*                 TO_CHAR                         (wchar_t* string);
    LDR_DATA_TABLE_ENTRY* GetLDREntry                     (std::string name);
    char*                 ScanModIn                       (char* pattern, char* mask, std::string modName);
    LDR_DATA_TABLE_ENTRY* GetLDREntryInternal             (const wchar_t* modName);
    char*                 GetModuleBaseAddressInternalPEB (const wchar_t* modName);
    uintptr_t             GetOffset                       (uintptr_t addr, int off);
    uintptr_t             GetOffsetNet                    (uintptr_t addr, int off);
    LPCWSTR getStringLpcwstrG(const std::string& str);
}
