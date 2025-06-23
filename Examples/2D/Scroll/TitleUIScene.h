#pragma once
#include "LarvaEngine/Core/UIScene.h"
#include "LarvaEngine/GameObjects/UI/Button.h"

namespace Example2DScroll
{

	class TitleUIScene : public UIScene
	{
	public:
		TitleUIScene(MainScene& parent);
		~TitleUIScene() override;

		void InputUI(const InputAction& input) override;
		void UpdateUI(float deltaTime) override;

	private:

		void LoadData() override {};
		void LoadObjects() override;

		ButtonGroup _buttonGroup;
	};
}