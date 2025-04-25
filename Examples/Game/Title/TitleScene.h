#pragma once
#include "LarvaEngine/Core/MainScene.h"	

namespace ExampleTitle {
	class TitleScene : public MainScene {
	public:
		TitleScene(SceneManager& manager);
		~TitleScene() override;


	private:

		void LoadData() override;

	};
}