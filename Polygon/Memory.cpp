#include "Memory.h"

void Patch(BYTE* destination, BYTE* source, size_t size)
{
	DWORD oldProtect;
	VirtualProtect((LPVOID)destination, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(destination, source, size);
	VirtualProtect(destination, size, oldProtect, &oldProtect);
}

NopInternal::NopInternal(BYTE* destination, size_t size)
	:destination{ destination }, size{ size }, originalCode{ new BYTE[size] }, nopCode{ new BYTE[size] }
{
	memset(nopCode, 0x90, size);
	memcpy(originalCode, destination, size);
}

void NopInternal::enable()
{
	Patch(destination, nopCode, size);
}

void NopInternal::disable()
{
	Patch(destination, originalCode, size);
}

NopInternal::~NopInternal()
{
	delete[] this->nopCode;
	delete[] this->originalCode;
}

PatchInternal::PatchInternal(BYTE* destination, BYTE* source, size_t size) : destination{ destination }, size{ size },
originalCode{ new BYTE[size] }, newCode{ new BYTE[size] }
{
	memcpy(newCode, source, size);
	memcpy(originalCode, destination, size);
}

void PatchInternal::enable()
{
	Patch(destination, newCode, size);
}

void PatchInternal::disable()
{
	Patch(destination, originalCode, size);
}

PatchInternal::~PatchInternal()
{
	delete[] this->originalCode;
	delete[] this->newCode;
}