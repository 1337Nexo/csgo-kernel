#include <iostream>
#include <future>

#include "main.h"
#include "game.h"
#include "keinterface.h"

inline void loop(manager game)
{
	while (TRUE)
	{
		game.glow();
		game.bhop();
	}
}

int main()
{
	SetConsoleTitle("notepad");
	SetConsoleCtrlHandler(control_handler, TRUE);

	if (load_driver())
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	manager game;

	std::thread loop(&loop, game);
	loop.detach();

	while (TRUE)
		if (GetAsyncKeyState(VK_END))
			exit(-1);
}

bool load_driver()
{
	std::error_code error;

	driver_name = random_string(6);
	driver_path = std::filesystem::absolute("driver/" + driver_name + ".sys");

	if (!std::filesystem::copy_file(std::filesystem::absolute(o_driver_path), driver_path, error))
	{
		std::cout << xorstr_("[*]: the driver can't be copied: %s\n") << error.message().c_str() << std::endl;
		return false;
	}

	if (!start_service(driver_name, driver_path.string()))
	{
		std::cout << xorstr_("[*]: start service failed..") << std::endl;
		return false;
	}

	std::cout << xorstr_("[*]: driver loaded..") << std::endl;

	return true;
}

void unload_driver()
{
	if (!stop_service(driver_name))
		std::cout << xorstr_("[*]: stop_service failed..") << std::endl;

	std::error_code error;

	if (!std::filesystem::remove(driver_path, error))
		std::cout << xorstr_("[*]: remove failed %s\n") << error.message().c_str() << std::endl;
}

BOOL WINAPI control_handler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_BREAK_EVENT:
		unload_driver();
		break;
	}

	return TRUE;
}
