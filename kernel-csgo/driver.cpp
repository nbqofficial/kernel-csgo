#include "dispatch.hpp"

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);
	hook::init_hook(&dispatch::handler);

	DbgPrintEx(0, 0, "[kernel-csgo]: Driver loaded\n");

	return STATUS_SUCCESS;
}