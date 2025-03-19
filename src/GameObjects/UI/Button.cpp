#include "LarvaEngine/GameObjects/UI/Button.h"
#include "LarvaEngine/Core/Events/Input.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"

#pragma region コンストラクタ

Button::Button(Scene* scene)
	: GameObject(scene)
	, _hoveredIndex(-1)
	, _offset(Vector2::Zero){

}

Button::~Button() {
	if (_spriteComp){
		delete _spriteComp;
		_spriteComp = nullptr;
	}
	if (_textComp) {
		delete _textComp;
		_textComp = nullptr;
	}

}

#pragma endregion

#pragma region パブリック関数

void Button::InputObject(Input* input) {
   
	if (_hoveredIndex != -1) {
		if (input->IsInputDown(InputMap::INPUT_BRIGHT)) {
			_buttonData[_hoveredIndex].onClick();
		}

		if (input->IsInputDown(InputMap::INPUT_DUP)) {
			if (_hoveredIndex > 0) _hoveredIndex--;
			else _hoveredIndex = _buttonData.size() - 1;
		}
		else if (input->IsInputDown(InputMap::INPUT_DDOWN)) {
			if (_hoveredIndex < _buttonData.size() - 1) _hoveredIndex++;
			else _hoveredIndex = 0;
		}
	}
	else {
		if (input->CheckInputAll()) {
			_hoveredIndex = 0;
		}
	}

}

void Button::UpdateObject(float deltaTime) {
	for (int i = 0; i < _buttonData.size(); i++) {
		if (i == _hoveredIndex) _buttonData[i].textComp->Color(Vector3(0.8,0.8,0.8));
		else  _buttonData[i].textComp->Color(Vector3(1, 1, 1));
	}
}

void Button::SetText(int index, const std::string& text, const std::string& fontPath, int pointSize, const Vector3& color) {
	if (index > _buttonData.size() - 1) return;
	_buttonData[index].textComp = new TextComponent(this, 0, fontPath);
	_buttonData[index].textComp->CreateTextTexture(text, color, pointSize);
	_buttonData[index].textComp->PositionOffset(index * _offset);
}

void Button::SetOnClick(int index, std::function<void()> onClick) {
	if (index > _buttonData.size() - 1) return;
	_buttonData[index].onClick = onClick;
}

void Button::SetOffset(const Vector2& offset) {
	_offset = offset;

	for (int i = 0; i < _buttonData.size();i++) {
		_buttonData[i].textComp->PositionOffset(offset * i);
	}
}

void Button::CreateButtonData() {
	ButtonData data;
	
	_buttonData.push_back(data);
}

#pragma endregion

#pragma region プライベート関数



#pragma endregion
