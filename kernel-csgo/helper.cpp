#include "helper.hpp"

void* helper::get_sys_module(const char* module_name)
{
	void* module_base = 0;
	ULONG bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, NULL, bytes, &bytes);

	if (!bytes) { return NULL; }

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x4e425151);

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	if (!NT_SUCCESS(status)) { return NULL; }

	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;

	for (ULONG i = 0; i < modules->NumberOfModules; i++)
	{
		if (strcmp((char*)module[i].FullPathName, module_name) == 0)
		{
			module_base = module[i].ImageBase;
			break;
		}
	}

	if (modules) { ExFreePoolWithTag(modules, 0x4e425151); }

	if (module_base <= 0) { return NULL; }

	return module_base;
}

void* helper::get_sys_module_export(const char* module_name, const char* function_name)
{
	void* module = get_sys_module(module_name);

	if (!module) { return NULL; }

	return RtlFindExportedRoutineByName(module, function_name);
}

bool helper::wpm_safe(void* address, void* buffer, size_t size)
{
	PMDL mdl = IoAllocateMdl(address, size, FALSE, FALSE, NULL);
	if (!mdl) { return false; }

	MmProbeAndLockPages(mdl, KernelMode, IoReadAccess);
	void* map = MmMapLockedPagesSpecifyCache(mdl, KernelMode, MmNonCached, NULL, FALSE, NormalPagePriority);
	MmProtectMdlSystemAddress(mdl, PAGE_READWRITE);

	memcpy(map, buffer, size);

	MmUnmapLockedPages(map, mdl);
	MmUnlockPages(mdl);
	IoFreeMdl(mdl);

	return true;
}
