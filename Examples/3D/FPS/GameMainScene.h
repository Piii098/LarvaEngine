#pragma once
#include "LarvaEngine/Core/MainScene.h"

namespace Example3DFPS
{

	class GameMainScene : public MainScene{
	public:
		GameMainScene(SceneManager& manager);
		~GameMainScene() override;

		
	private:
		
		void LoadData() override;
		void LoadObjects() override;

	};
}