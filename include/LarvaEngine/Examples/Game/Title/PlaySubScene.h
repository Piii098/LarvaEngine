#pragma once
#include "LarvaEngine/Core/SubScene.h"

namespace ExampleTitle{

	class PlaySubScene : public SubScene {
	public:
		PlaySubScene(MainScene* parent);
		~PlaySubScene() override;

		void Initialize() override;

		void InputScene(Input* input) override;

		void UpdateScene(float deltaTime) override;


	};

}