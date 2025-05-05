#ifndef BUILD_AS_LIBRARY
#include "LarvaEngine/Core/Game.h"
#include "Examples/Game/2DScroll/TitleMainScene.h"
#include "Examples/Game/2DScroll/TestMainScene.h"
#include "Examples/Game/2DScroll/GameMainScene.h"

int main(int argc, char* argv[]) {
	
	Game game;
	game.SetInitialScene<Example2DScroll::TitleMainScene>();
	game.SetWindowsSize(1440, 810, 480, 270);
	game.SetWindowsName("sample");
	bool success = game.Initialize();

	if (success) {
		game.RunLoop();
	}

	game.Shutdown();
	return 0;
	
}

#endif