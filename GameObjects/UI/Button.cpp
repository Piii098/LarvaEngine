#include "GameObjects/UI/Button.h"
#include "Utilities/Input.h"
#include "Components/Draw/TextComponent.h"

#pragma region コンストラクタ

Button::Button(Scene* scene)
	: GameObject(scene){

}

Button::~Button() {
}

#pragma endregion

#pragma region パブリック関数

void Button::InputObject(Input* input) {

	if (_isHovered && input->IsInputDown(InputMap::INPUT_DRIGHT)) {
		_onClick();
	}

}

void Button::SetText(const std::string& text, const std::string& fontPath, int pointSize, const Vector3& color) {
	_textComp = new TextComponent(this, 0, fontPath);
	_textComp->CreateTextTexture(text, color, pointSize);
}


#pragma endregion

#pragma region プライベート関数



#pragma endregion
