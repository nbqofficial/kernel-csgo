#pragma once

#include "hook.hpp"
#include <TlHelp32.h>

namespace memory
{
	ULONG get_process_id_by_name(const char* process_name);
}