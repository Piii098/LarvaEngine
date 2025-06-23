#ifndef BUILD_AS_LIBRARY
#include "LarvaEngine/Core/Game.h"
#include "Examples/2D/Scroll/TitleMainScene.h"
#include "Examples/2D/Scroll/GameMainScene.h"
#include "Examples/3D/Sample/SampleMainScene.h"
#include "Examples/3D/FPS/GameMainScene.h"

int main(int argc, char* argv[]) {
	
	Game game;
	game.RendererMode(Game::RENDERER_MODE::MODE_2D); // 2D or 3D
	game.SetInitialScene<Example2DScroll::TitleMainScene>();
	game.SetWindowsSize(1920, 1080);
	game.SetWindowsName("sample");
	bool success = game.Initialize();

	if (success) {
		game.RunLoop();
	}

	game.Shutdown();
	return 0;
	
}

#endif