#pragma once
#include "LarvaEngine/Core/MainScene.h"

class Player;

namespace Example {

	class TestScene : public MainScene {
	public:
		TestScene(SceneManager& manager);
		~TestScene() override;

		void Initialize() override;

		void LoadData() override;

		Player* GetPlayer() const { return _player; }

	private:
		Player* _player;
	};

}
