#include "cheat.hpp"

int main()
{
	printf("[kernel-csgo]: Loading necessary modules...\n");
	LoadLibraryA("user32.dll");
	LoadLibraryA("win32u.dll");
	if (GetModuleHandleA("win32u.dll"))
	{
		printf("[kernel-csgo]: Modules loaded\n");
		cheat::run();
	}
	else
	{
		printf("[kernel-csgo]: Modules failed to load, quitting\n");
	}

	return 0;
}