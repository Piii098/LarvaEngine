#pragma once
#include "LarvaEngine/Core/UIScene.h"

class DataText;
class Text;

namespace Example {

	class HUD : public UIScene {
	public:
		HUD(MainScene& parent);
		~HUD() override;

		//void InputUI(const InputAction& input) override;

		//void UpdateUI(float deltaTime) override;

	private:
		void LoadData() override;
		DataText* _dataText;
		Text* _text;
	};

}