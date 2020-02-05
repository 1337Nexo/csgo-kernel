#include <iostream>
#include <future>

#include "main.h"
#include "game.h"
#include "keinterface.h"

inline void featuresThread(manager game)
{
	while (TRUE)
	{
		game.doGlow();
		game.doBhop();
	}
}

int main()
{
	SetConsoleTitle(" ");

	manager game;

	std::thread features(&featuresThread, game);
	features.detach();

	while (TRUE)
		if (GetAsyncKeyState(VK_END))
			exit(-1);
}
