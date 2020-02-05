#pragma once
#include <iostream>
#include "keinterface.h"

KeInterface driver("\\\\.\\driver");

namespace offsets
{
	constexpr uintptr_t dwLocalPlayer = 0xCFAA2C;
	constexpr uintptr_t m_fFlags = 0x104;
	constexpr uintptr_t dwForceJump = 0x51B0708;
	constexpr uintptr_t dwEntityList = 0x4D0D054;
}

class manager
{
public:
	DWORD pid;
	DWORD client_address;

	manager()
	{
		pid = driver.get_pid();

		std::cout << xorstr_("[*]: pid: ") << pid << std::endl;

		client_address = driver.get_module();

		std::cout << xorstr_("[*]: client_address: ") << std::uppercase << std::hex << client_address << std::endl;
	}

	DWORD in_ground()
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

				driver.Write(pid, dwCurrentEntity + 0x3960, flSensorTime, sizeof(float));
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		return TRUE;
	}

	bool bhop()
	{
		if (!local_player())
			return 0;

		if (!in_ground())
			return 0;

		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (in_ground()))
		{
			driver.Write(pid, client_address + offsets::dwForceJump, 0x5, 8);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			driver.Write(pid, client_address + offsets::dwForceJump, 0x4, 8);

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		return TRUE;
	}
};
