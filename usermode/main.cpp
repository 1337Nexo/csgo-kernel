#include <iostream>
#include <future>

#include "main.h"
#include "game.h"
#include "keinterface.h"

inline void features_thread(game game)
{
	while (TRUE)
	{
		game.glow();
		game.bhop();
	}
}

int main()
{
	game game;

	std::thread features(&features_thread, game);
	features.detach();

	while (TRUE)
		if (GetAsyncKeyState(VK_END))
			exit(-1);
}
