#pragma once
#include "LarvaEngine/Core/MainScene.h"

namespace Exaple2DScroll
{
	class Player;
	class InputAction;
	class PauseMenu;
	class GameOverMenu;
	class GameClearMenu;

	class GameMainScene : public MainScene
	{
	public:
		GameMainScene(SceneManager& manager);
		~GameMainScene() override;

		void Initialize() override;
		void LoadData() override;

	private:
		Player* _player;
		PauseMenu* _pauseMenu;
		GameOverMenu* _gameOverMenu;
		GameClearMenu* _gameClearMenu;
	};
}