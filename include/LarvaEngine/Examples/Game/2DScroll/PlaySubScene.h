#pragma once
#include "LarvaEngine/Core/SubScene.h"

namespace Example2DScroll {

	class Player;

	class PlaySubScene : public SubScene {
	public:
		PlaySubScene(MainScene& mainScene, Player& player);
		~PlaySubScene() override;

		void Initialize() override;
	private:
		Player& _player;
	};

}