#ifndef BUILD_AS_LIBRARY
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Examples/Game/Title/TitleScene.h"
#include "LarvaEngine/Examples/Game/Test/TestScene.h"

int main(int argc, char* argv[]) {
	
	Game game;
	game.SetInitialScene<Example::TestScene>();
	game.SetWindowsSize(1280, 720, 640, 360);
	game.SetWindowsName("Doppel");
	bool success = game.Initialize();

	if (success) {
		game.RunLoop();
	}

	game.Shutdown();
	return 0;
	
}

#endif