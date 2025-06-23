#pragma once
#include "LarvaEngine/Core/MainScene.h"

class Player;

namespace Example2DScroll
{
	class GameMainScene : public MainScene
	{
	public:
		GameMainScene(SceneManager& manager);
		~GameMainScene() override;

	private:

		void UpdateScene(float deltaTime) override;

		void InputScene(const InputAction& input) override;

		void LoadData() override;
		void LoadObjects() override;
		Player* _player;

		bool _isMenu;

	};
}