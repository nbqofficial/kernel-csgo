#pragma once

#include "hook.hpp"

namespace memory
{
	ULONG get_module_base(PEPROCESS process, LPCWSTR module_name);

	NTSTATUS read_memory(PEPROCESS target_process, void* source, void* target, size_t size);

	NTSTATUS write_memory(PEPROCESS target_process, void* source, void* target, size_t size);
}