#pragma once
#include "LarvaEngine/Core/SubScene.h"

class GameObject;

namespace Sample3D
{
	class Sample3DSubScene : public SubScene
	{
	public:
		Sample3DSubScene(MainScene& scene, GameObject& cube);
		~Sample3DSubScene() override;

	
	private:
		GameObject& _cube;

		void UpdateScene(float deltaTime) override;
	};
}