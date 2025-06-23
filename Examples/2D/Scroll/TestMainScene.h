#pragma once
#include "LarvaEngine/Core/MainScene.h"

namespace Example2DScroll
{
	class TestMainScene : public MainScene
	{
	public:
		TestMainScene(SceneManager& manager);
		~TestMainScene() override;

	private:

		void LoadData() override;
		void LoadObjects() override;
	};
}