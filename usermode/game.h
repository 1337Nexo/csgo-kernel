#pragma once
#include <iostream>
#include "keinterface.h"

KeInterface driver("\\\\.\\csgokernel");

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
	DWORD clientAddress;

	manager()
	{
		pid = driver.get_pid();

		std::cout << XorStr("[procId]: ") << pid << std::endl;

		clientAddress = driver.get_module();

		std::cout << XorStr("[clientAddress]: ") << std::uppercase << std::hex << clientAddress << std::endl;
	}

	DWORD inGround()
	{
		return driver.Read<DWORD>(pid, localPlayer() + offsets::m_fFlags, sizeof(ULONG));
	}

	DWORD localPlayer()
	{
		return driver.Read<DWORD>(pid, clientAddress + offsets::dwLocalPlayer, sizeof(ULONG));;
	}
	
	bool doGlow()
	{
		for (int i = 1; i < 65; i++)
		{
			int dwCurrentEntity = driver.Read<int>(pid, clientAddress + offsets::dwEntityList + (i * 16), sizeof(int));

			if (dwCurrentEntity)
			{
				float flSensorTime = 86400.f;

				driver.Write(pid, dwCurrentEntity + 0x3960, flSensorTime, sizeof(float));
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		return TRUE;
	}

	bool doBhop()
	{
		if (!localPlayer())
			return FALSE;

		if (!inGround())
			return FALSE;

		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (inGround()))
		{
			driver.Write(pid, clientAddress + offsets::dwForceJump, 0x5, 8);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			driver.Write(pid, clientAddress + offsets::dwForceJump, 0x4, 8);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		return TRUE;
	}
};
