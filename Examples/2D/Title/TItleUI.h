#pragma once
#include "LarvaEngine/Core/UIScene.h"

class Button;

namespace ExampleTitle
{

	class TitleUI : public UIScene {
	public:
		TitleUI(MainScene& scene);
		~TitleUI() override;

		//void Initialize() override;
		void UpdateScene(float deltaTime) override;

	private:

		void LoadData() override;

		Button* _button;

	};
}