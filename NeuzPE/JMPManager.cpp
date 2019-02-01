#include "JMPManager.h"
#include <Windows.h>
#include <cstdint>

JMP::JMP(void* from, void* to) {
	this->from = from;
	this->to = to;
	this->active = false;
}

void JMP::Activate() {

	this->original_data = *(uint64_t*)from;
	this->make_writable();

	size_t jmp_instruction_size = 5;

	// Make a simple jmp patch/hook
	int32_t relativeOffset = (((int32_t)this->to - (int32_t)this->from) - jmp_instruction_size);
	*(BYTE*)this->from = 0xE9;
	*(int32_t*)((uint32_t)this->from + 1) = relativeOffset;

	this->restore_perms();

	// Flush the CPU instruction cache because we changed some instructions. Just in case.
	FlushInstructionCache(GetCurrentProcess(), this->from, jmp_instruction_size);

	this->active = true;
}

void JMP::Restore() {
	this->make_writable();
	*(uint64_t*)from = this->original_data;
	this->restore_perms();
	this->active = false;
}

void JMP::make_writable() {
	// Change memory permissions
	VirtualProtect((void*)this->from, 8, PAGE_EXECUTE_READWRITE, (PDWORD)&this->original_perms);
}

void JMP::restore_perms() {
	// Restore memory permissions
	DWORD tmp;
	VirtualProtect((void*)this->from, 8, this->original_perms, &tmp);
}