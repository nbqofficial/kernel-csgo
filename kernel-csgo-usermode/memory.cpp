#include "memory.hpp"

ULONG memory::get_process_id_by_name(const char* process_name)
{
	ULONG process_id = 0;
	HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (ss)
	{
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(ss, &pe))
		{
			do
			{
				if (!strcmp(pe.szExeFile, process_name))
				{
					process_id = pe.th32ProcessID;
					break;
				}
			} while (Process32Next(ss, &pe));
		}
		CloseHandle(ss);
	}
	return process_id;
}
