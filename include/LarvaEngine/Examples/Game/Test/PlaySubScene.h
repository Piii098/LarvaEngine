#pragma once
#include "LarvaEngine/Core/SubScene.h"

class Player;
class InputAction;

namespace Example {

	class PlaySubScene : public SubScene {
	public:
		PlaySubScene(MainScene& parent, Player* player);
		~PlaySubScene() override;

		void Initialize() override;

		void InputScene(const InputAction& input) override;
		void UpdateScene(float deltaTime) override;
	private:
		Player* _player;
	};

}