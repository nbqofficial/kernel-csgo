#include "cheat.hpp"

void cheat::run()
{
	printf("[kernel-csgo]: Looking for csgo.exe process...\n");
	ULONG process_id = memory::get_process_id_by_name("csgo.exe");
	if (process_id)
	{
		printf("[kernel-csgo]: Process identifier found: %d\n", process_id);
		printf("[kernel-csgo]: Looking for client.dll...\n");
		ULONG client_base = driver_control::get_client_base(process_id);
		if (client_base)
		{
			printf("[kernel-csgo]: client.dll found: 0x%x\n", client_base);
			printf("[kernel-csgo]: Running...\n");

			for (;;)
			{
				ULONG my_player = driver_control::read_memory<ULONG>(process_id, client_base + m_dwLocalPlayer);
				if (my_player <= 0) { continue; }

				ULONG my_team = driver_control::read_memory<ULONG>(process_id, my_player + m_iTeamNum);
				ULONG my_cross = driver_control::read_memory<ULONG>(process_id, my_player + m_iCrossHairID);

				if (my_cross >= 1 && my_cross <= 64)
				{
					ULONG entity = driver_control::read_memory<ULONG>(process_id, client_base + m_dwEntityList + ((my_cross - 1) * 0x10));
					if (entity <= 0) { continue; }

					ULONG entity_health = driver_control::read_memory<ULONG>(process_id, entity + m_iHealth);
					ULONG entity_team = driver_control::read_memory<ULONG>(process_id, entity + m_iTeamNum);

					if (entity_health > 0 && entity_team != my_team && entity_team > 1)
					{
						Sleep(3);
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						Sleep(1);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
				}
				Sleep(1);
			}
		}
		else
		{
			printf("[kernel-csgo]: Failed to obtain client.dll\n");
		}
	}
	else
	{
		printf("[kernel-csgo]: Failed to find csgo.exe process\n");
	}
}
