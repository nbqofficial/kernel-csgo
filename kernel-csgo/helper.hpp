#pragma once

#include "definitions.hpp"

namespace helper
{
	void* get_sys_module(const char* module_name);

	void* get_sys_module_export(const char* module_name, const char* function_name);

	bool wpm_safe(void* address, void* buffer, size_t size);
}
