#include <SDL3/SDL.h>
#include "LarvaEngine/GameObjects/UI/Button.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"
#include "LarvaEngine/Components/Physics/BoxComponent2D.h"
#include "LarvaEngine/Components/UI/ButtonComponent.h"
#include "LarvaEngine/Input/InputAction.h"

/// ============================================================================
/// ButtonGroup クラス
/// ============================================================================

ButtonGroup::ButtonGroup()
	: _currentSelection(-1)
	, _inputState(INPUT_STATE::MOUSE){

}

ButtonGroup::~ButtonGroup() {

}	

void ButtonGroup::AddButton(ButtonComponent* button) {
	_buttons.push_back(button);

}

void ButtonGroup::InputProcess(const InputAction& input) {

	if (_inputState == INPUT_STATE::MOUSE) {
		InputMouse(input);
		if (input.IsAllKey() || input.IsAllGamepadButton()) {
			_inputState = INPUT_STATE::KEY_BUTTON;
		}
	}
	else if (_inputState == INPUT_STATE::KEY_BUTTON) {
		InputKeyAndButton(input);
		if (CheckHover() != -1) {
			_inputState = INPUT_STATE::MOUSE;
		}
	}


}

void ButtonGroup::InputKeyAndButton(const InputAction& input) {
	if (_currentSelection == -1) _currentSelection = 0;
	
	MoveFocus(input);
}

void ButtonGroup::InputMouse(const InputAction& input) {
	_currentSelection = CheckHover();

	UpdateSelection();
}

void ButtonGroup::MoveFocus(const InputAction& input) {
	int prevSelection = _currentSelection;

	if (input.IsActionDown("Down")) {
		_currentSelection = (_currentSelection + 1) % _buttons.size();
	}
	else if (input.IsActionDown("Up")) {
		_currentSelection = (_currentSelection - 1 + _buttons.size()) % _buttons.size();
	}

	// SDL_Log("%d", _currentSelection);
	UpdateSelection();
}

void ButtonGroup::ConfirmSelection(const InputAction& input) {
	if (_currentSelection >= 0 && _currentSelection < _buttons.size()) {
	   
	}
}

int ButtonGroup::CheckHover() {
	for (int i = 0; i < _buttons.size(); ++i) {
		if (_buttons[i]->IsHovered()) {
			return i;
		}
	}
	return -1;
}

void ButtonGroup::SetInitialSelection(int index) {
	if (index >= 0 && index < _buttons.size()) {
		_currentSelection = index;
		UpdateSelection();
	}
}

void ButtonGroup::UpdateSelection() {
	for (int i = 0; i < _buttons.size(); ++i) {
		_buttons[i]->SetSelected(i == _currentSelection);
	}

}

// ==============================================================================
// コンストラクタ・デストラクタ
// ==============================================================================

Button::Button(Scene& scene, const std::string& spriteName)
	: GameObject(scene)
{
	SpriteComponent& spri = CreateComponent<SpriteComponent>(10);
	spri.SetTexture(spriteName);
	TextComponent& text = CreateComponent<TextComponent>(10, "MOBO");
	BoxComponent2D& box = CreateComponent<BoxComponent2D>(false, false);

	ButtonComponent& button = CreateComponent<ButtonComponent>();

	_spriteComponent = GetComponent<SpriteComponent>();
	_textComponent = GetComponent<TextComponent>();
	_buttonComponent = GetComponent<ButtonComponent>();

}

Button::~Button() {
	
}

// =============================================================================
// パブリック関数
// =============================================================================

void Button::AddToGroup(ButtonGroup& group) {
	group.AddButton(_buttonComponent);
}

void Button::SetText(const std::string& text, const Vector3& color, int pointSize) {

	_textComponent->CreateTextTexture(text, color, pointSize);

}

void Button::SetOnClick(std::function<void()> callback) {
	_buttonComponent->SetOnClickCallback(callback);
}

void Button::SetOnHover(std::function<void()> callback) {
	
}
