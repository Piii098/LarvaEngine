#pragma once
#include "Scene/SubScene.h"

class Player;

namespace test {
	
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