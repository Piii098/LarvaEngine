#pragma once
#include "LarvaEngine/Core/UIScene.h"
#include "LarvaEngine/GameObjects/UI/Button.h"

namespace Example2DScroll
{
	class MenuUIScene : public UIScene
	{
	public:
		MenuUIScene(MainScene& parent);
		~MenuUIScene() override;

		void InputUI(const InputAction& input) override;

	private:
		void LoadData() override;
		void LoadObjects() override;

		ButtonGroup _buttonGroup;
	};
}