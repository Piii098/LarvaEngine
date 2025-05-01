#pragma once
#include "LarvaEngine/Core/UIScene.h"

namespace Example2DScroll
{
	class MenuUIScene : public UIScene
	{
	public:
		MenuUIScene(MainScene& parent);
		~MenuUIScene() override;

	private:
		void LoadData() override;
		void LoadObjects() override;
	};
}