#include "memory.hpp"

ULONG memory::get_module_base(PEPROCESS process, LPCWSTR module_name)
{
	if (!process) { return 0; }

	__try
	{
		PPEB32 peb32 = (PPEB32)PsGetProcessWow64Process(process);
		if (!peb32 || !peb32->Ldr) { return 0; }

		for (PLIST_ENTRY32 plist_entry = (PLIST_ENTRY32)((PPEB_LDR_DATA32)peb32->Ldr)->InLoadOrderModuleList.Flink;
			plist_entry != &((PPEB_LDR_DATA32)peb32->Ldr)->InLoadOrderModuleList;
			plist_entry = (PLIST_ENTRY32)plist_entry->Flink)
		{
			PLDR_DATA_TABLE_ENTRY32 pentry = CONTAINING_RECORD(plist_entry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

			if (wcscmp((PWCH)pentry->BaseDllName.Buffer, module_name) == 0)
			{
				return pentry->DllBase;
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{

	}

	return 0;
}

NTSTATUS memory::read_memory(PEPROCESS target_process, void* source, void* target, size_t size)
{
	if (!target_process) { return STATUS_INVALID_PARAMETER; }

	size_t bytes = 0;
	NTSTATUS status = MmCopyVirtualMemory(target_process, source, IoGetCurrentProcess(), target, size, KernelMode, &bytes);
	if (!NT_SUCCESS(status) || !bytes)
	{
		return STATUS_INVALID_ADDRESS;
	}
	return status;
}

NTSTATUS memory::write_memory(PEPROCESS target_process, void* source, void* target, size_t size)
{
	if (!target_process) { return STATUS_INVALID_PARAMETER; }

	size_t bytes = 0;
	NTSTATUS status = MmCopyVirtualMemory(IoGetCurrentProcess(), source, target_process, target, size, KernelMode, &bytes);
	if (!NT_SUCCESS(status) || !bytes)
	{
		return STATUS_INVALID_ADDRESS;
	}
	return status;
}
