#pragma once
#include "LarvaEngine/Core/MainScene.h"

namespace Example2DScroll
{

	class GameMainScene : public MainScene
	{
	public:
		GameMainScene(SceneManager& manager) ;
		~GameMainScene() override {};

		void LoadData() override {};

	private:
		GameObject* _player;
		GameObject* _pauseMenu;
		GameObject* _gameOverMenu;
		GameObject* _gameClearMenu;
	};
}