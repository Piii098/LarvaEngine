#pragma once
#include "LarvaEngine/Core/UIScene.h"

class Text;
class Player;
class InputAction;

namespace Example
{

	class PauseMenu : public UIScene {
	public:
		PauseMenu(MainScene& parent);
		~PauseMenu() override;

		void Initialize() override;
		void InputUI(const InputAction& input) override;
		//virtual void UpdateScene(float deltaTime) override ;

	private:
		void LoadData() override;
		Text* _text;
		Player* _player;
	};

}