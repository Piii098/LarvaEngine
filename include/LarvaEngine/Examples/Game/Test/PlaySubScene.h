#pragma once
#include "LarvaEngine/Core/SubScene.h"

class Player;

namespace Example {

	class PlaySubScene : public SubScene {
	public:
		PlaySubScene(MainScene* parent, Player* player);
		~PlaySubScene() override;

		void Initialize() override;

		void InputScene(Input* input) override;
		void UpdateScene(float deltaTime) override;
	private:
		Player* _player;
	};

}