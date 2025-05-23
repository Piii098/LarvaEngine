#pragma once
#include "LarvaEngine/Core/UIScene.h"

namespace Example3DFPS {

	class HUDUIScene : public UIScene {
	public:
		HUDUIScene(MainScene& parent);
		~HUDUIScene() override;

		void LoadData() override;
		void LoadObjects() override;

	private:

	};
}