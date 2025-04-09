#ifndef BUILD_AS_LIBRARY
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Examples/Game/Title/TitleScene.h"
#include "LarvaEngine/Examples/Game/Test/TestScene.h"

int main(int argc, char* argv[]) {
	
	Game game;
	game.SetInitialScene<Example::TestScene>();
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