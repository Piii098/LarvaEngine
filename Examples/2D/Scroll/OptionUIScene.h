#pragma once
#include "LarvaEngine/Core/UIScene.h"
#include "LarvaEngine/GameObjects/UI/Button.h"

namespace Example2DScroll
{
	class OptionUIScene : public UIScene
	{
	public:
		OptionUIScene(MainScene& parent);
		~OptionUIScene() override;

		
		void InputUI(const InputAction& input) override;
		void UpdateUI(float deltaTime) override;

	private:
		void LoadData() override {};
		void LoadObjects() override;

		ButtonGroup _buttonGroup;
	};
}