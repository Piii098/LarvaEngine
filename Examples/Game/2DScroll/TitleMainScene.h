#pragma once
#include "LarvaEngine/Core/MainScene.h"

namespace Example2DScroll
{

	class TitleMainScene : public MainScene
	{
	public:
		TitleMainScene(SceneManager& manager);
		~TitleMainScene() override;

		void LoadData() override;

	private:

	};

}