#pragma once
#include "LarvaEngine/Core/MainScene.h"

namespace Example2DScroll
{
	class Player;
	class GameMainScene : public MainScene
	{
	public:
		GameMainScene(SceneManager& manager) ;
		~GameMainScene() override;

		void LoadData() override;

	private:
		Player* _player;

	};
}