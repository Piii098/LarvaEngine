#pragma once
#include "LarvaEngine/Core/UIScene.h"

class SpriteComponent;

namespace Example2DScroll
{
	class HUDUIScene : public UIScene
	{
	public:
		HUDUIScene(MainScene& parent);
		~HUDUIScene() override;

		void UpdateUI(float deltaTime) override;

	private:
		void LoadData() override;
		void LoadObjects() override;

		SpriteComponent* _playerBar;
		Vector2 _playerBarVec;
	};
}
