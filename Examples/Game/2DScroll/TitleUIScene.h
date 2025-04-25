#pragma once
#include "LarvaEngine/Core/UIScene.h"

namespace Example2DScroll
{
	class TitleUIScene : public UIScene
	{
	public:
		TitleUIScene(MainScene& parent);
		~TitleUIScene() override;

	private:

		void LoadData() override;
	};
}