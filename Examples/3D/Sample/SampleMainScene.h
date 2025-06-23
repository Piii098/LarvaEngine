#pragma once
#include "LarvaEngine/Core/MainScene.h"

namespace Sample3D
{
	class SampleMainScene : public MainScene{
	public:
		SampleMainScene(SceneManager& manager);
		~SampleMainScene();

	private:

		void LoadData() override;
		void LoadObjects() override;

	};
}