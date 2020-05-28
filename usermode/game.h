#pragma once
#include <iostream>
#include "parson.h"
#include "keinterface.h"

KeInterface driver("\\\\.\\csgokernel");

namespace offsets
{
	static uintptr_t dwLocalPlayer;
	static uintptr_t m_fFlags;
	static uintptr_t dwForceJump;
	static uintptr_t dwEntityList;
	static uintptr_t m_bGunGameImmunity;
}

class game
{
public:
	DWORD pid;
	DWORD client_address;
	JSON_Value* root_value;
	JSON_Object* base;
	JSON_Object* signatures;
	JSON_Object* netvars;

	game()
	{
		pid = driver.get_pid();
		client_address = driver.get_module();

		{
			root_value = json_parse_file("hazedumper/csgo.min.json");
			if (!root_value)
				return;
			base = json_value_get_object(root_value);
			signatures = json_object_get_object(base, "signatures");
			netvars = json_object_get_object(base, "netvars");
			offsets::dwEntityList = (DWORD)json_object_get_number(signatures, "dwEntityList");
			offsets::dwLocalPlayer = (DWORD)json_object_get_number(signatures, "dwLocalPlayer");
			offsets::dwForceJump = (DWORD)json_object_get_number(signatures, "dwForceJump");
			offsets::m_fFlags = (DWORD)json_object_get_number(netvars, "m_fFlags");
			offsets::m_bGunGameImmunity = (DWORD)json_object_get_number(netvars, "m_bGunGameImmunity");
			json_value_free(root_value);
		}

		std::cout << xor_str("[process_id]: ") << pid << std::endl;
		std::cout << xor_str("[client_address]: ") << std::uppercase << std::hex << client_address << std::endl;
		std::cout << xor_str("[dwEntityList]: 0x") << std::uppercase << std::hex << offsets::dwEntityList << std::endl;
		std::cout << xor_str("[dwLocalPlayer]: 0x") << std::uppercase << std::hex << offsets::dwLocalPlayer << std::endl;
		std::cout << xor_str("[dwForceJump]: 0x") << std::uppercase << std::hex << offsets::dwForceJump << std::endl;
		std::cout << xor_str("[m_fFlags]: 0x") << std::uppercase << std::hex << offsets::m_fFlags << std::endl;
		std::cout << xor_str("[m_bGunGameImmunity]: 0x") << std::uppercase << std::hex << offsets::m_bGunGameImmunity << std::endl;
	}

	DWORD is_in_ground()
	{
		return driver.Read<DWORD>(pid, local_player() + offsets::m_fFlags, sizeof(ULONG));
	}

	DWORD local_player()
	{
		return driver.Read<DWORD>(pid, client_address + offsets::dwLocalPlayer, sizeof(ULONG));;
	}
	
	bool glow()
	{
		for (int i = 1; i < 65; i++)
		{
			int dwCurrentEntity = driver.Read<int>(pid, client_address + offsets::dwEntityList + (i * 16), sizeof(int));

			if (dwCurrentEntity)
			{
				float flSensorTime = 86400.f;

				driver.Write(pid, dwCurrentEntity + (offsets::m_bGunGameImmunity + 0x34), flSensorTime, sizeof(float));
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		return TRUE;
	}

	bool bhop()
	{
		if (!local_player())
			return FALSE;

		if (!is_in_ground())
			return FALSE;

		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (is_in_ground()))
		{
			driver.Write(pid, client_address + offsets::dwForceJump, 0x5, 8);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			driver.Write(pid, client_address + offsets::dwForceJump, 0x4, 8);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		return TRUE;
	}
};
