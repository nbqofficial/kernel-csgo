#include "hook.hpp"

bool hook::init_hook(void* kernel_address)
{
	if (!kernel_address) { return false; }

	void** function = reinterpret_cast<void**>(helper::get_sys_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys", "NtQueryCompositionSurfaceStatistics"));
	if (!function) { return false; }

	unsigned char mov_rax[] = { 0x48, 0xB8 };
	unsigned char jmp_rax[] = { 0xFF, 0xE0 };

	unsigned char original_fn[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	RtlSecureZeroMemory(&original_fn, sizeof(original_fn));
	memcpy((void*)((ULONG_PTR)original_fn), &mov_rax, sizeof(mov_rax));

	uintptr_t hook_address = reinterpret_cast<uintptr_t>(kernel_address);
	memcpy((void*)((ULONG_PTR)original_fn + sizeof(mov_rax)), &hook_address, sizeof(void*));
	memcpy((void*)((ULONG_PTR)original_fn + sizeof(mov_rax) + sizeof(void*)), &jmp_rax, sizeof(jmp_rax));

	helper::wpm_safe(function, &original_fn, sizeof(original_fn));
	
	return true;
}
