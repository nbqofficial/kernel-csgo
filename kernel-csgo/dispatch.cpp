#include "dispatch.hpp"

void dispatch::handler(void* info_struct)
{
	PINFO_STRUCT info = (PINFO_STRUCT)info_struct;

	if (info->code == CODE_CLIENT_REQUEST)
	{
		PEPROCESS target_process = NULL;
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)info->process_id, &target_process)))
		{
			KAPC_STATE apc;
			KeStackAttachProcess(target_process, &apc);
			ULONG b = memory::get_module_base(target_process, L"client.dll");
			KeUnstackDetachProcess(&apc);
			if (b) { info->client_base = b; }
		}
	}
	else if (info->code == CODE_READ_MEMORY)
	{
		PEPROCESS target_process = NULL;
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)info->process_id, &target_process)))
		{
			memory::read_memory(target_process, (void*)info->address, &info->buffer, info->size);
		}
	}
	else if (info->code == CODE_WRITE_MEMORY)
	{
		PEPROCESS target_process = NULL;
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)info->process_id, &target_process)))
		{
			memory::write_memory(target_process, &info->buffer, (void*)info->address, info->size);
		}
	}
}

