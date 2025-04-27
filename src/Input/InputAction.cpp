#include <SDL3/SDL.h>
#include <algorithm>
#include "LarvaEngine/Input/InputAction.h"
#include "LarvaEngine/Input/InputSystem.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Renderer/Renderer.h"


#pragma region コンストラクタ・デストラクタ

InputAction::InputAction(InputSystem& inputSystem)
	: _inputSystem(inputSystem){
	
}

InputAction::~InputAction()
{
}

#pragma endregion



#pragma region パブリックメソッド


void InputAction::MapAction(const std::string& name, GameTypes::InputCode code, int playerIndex) {
	_actionMap[name].push_back(code);
}

bool InputAction::IsAction(const std::string& name, int player) const {
	auto iter = _actionMap.find(name);
	if (iter == _actionMap.end()) {
		return false;
	}

	const std::vector<GameTypes::InputCode>& codes = iter->second;

	// キーボード入力のチェック
	for (const auto& code : codes) {
		if (std::holds_alternative<SDL_Scancode>(code) &&
			IsKey(std::get<SDL_Scancode>(code))) {
			return true;
		}
	}

	// マウス入力のチェック
	for (const auto& code : codes) {
		if (std::holds_alternative<GameTypes::SDL_MouseButton>(code) &&
			IsMouseButton(std::get<GameTypes::SDL_MouseButton>(code))) {
			return true;
		}
	}

	// ゲームパッド入力のチェック
	for (const auto& code : codes) {
		if (std::holds_alternative<SDL_GamepadButton>(code) &&
			IsGamepadButton(std::get<SDL_GamepadButton>(code))) {
			return true;
		}
	}

	return false;
}

void InputAction::MapActionAxis(const std::string& name, GameTypes::InputCode negative, GameTypes::InputCode positive) {
	_inputSystem.SetAxisMapping(name, negative, positive);
	_axisMap[name].push_back(name);
}

void InputAction::MapActionAxis(const std::string& name, SDL_GamepadAxis axis) {
	_axisMap[name].push_back(axis);
}

bool InputAction::IsActionDown(const std::string& name, int player) const {	
	auto iter = _actionMap.find(name);
	if (iter == _actionMap.end()) {
		return false;
	}

	const std::vector<GameTypes::InputCode>& codes = iter->second;

	// キーボード入力のチェック
	for (const auto& code : codes) {
		if (std::holds_alternative<SDL_Scancode>(code) &&
			IsKeyDown(std::get<SDL_Scancode>(code))) {
			return true;
		}
	}

	// マウス入力のチェック
	for (const auto& code : codes) {
		if (std::holds_alternative<GameTypes::SDL_MouseButton>(code) &&
			IsMouseButtonDown(std::get<GameTypes::SDL_MouseButton>(code))) {
			return true;
		}
	}

	// ゲームパッド入力のチェック
	for (const auto& code : codes) {
		if (std::holds_alternative<SDL_GamepadButton>(code) &&
			IsGamepadButtonDown(std::get<SDL_GamepadButton>(code))) {
			return true;
		}
	}

	return false;
}

bool InputAction::IsActionUp(const std::string& name, int player) const {
	// 実装を追加
	return false;
}

float InputAction::GetActionValue(const std::string& name, int player) const {
	auto iter = _axisMap.find(name);
	if (iter == _axisMap.end()) {
		return 0.0f;
	}
	
	const std::vector<GameTypes::AxisCode>& codes = iter->second;

	float value = 0.0f;

	for (const auto& code : codes) {
		if (std::holds_alternative<std::string>(code)) {
			value = Math::Abs(value) > Math::Abs(_inputSystem.GetAxis(std::get<std::string>(code))) ? value : _inputSystem.GetAxis(std::get<std::string>(code));
		}
	}

	for (const auto& code : codes) {
		if (std::holds_alternative<SDL_GamepadAxis>(code)) {
			value = Math::Abs(value) > Math::Abs(GetGamepadAxis(std::get<SDL_GamepadAxis>(code))) ? value : GetGamepadAxis(std::get<SDL_GamepadAxis>(code));
		}
	}

	return value;
}

Vector2 InputAction::GetActionVector(const std::string& name, int player) const {
	// 実装を追加
	return Vector2::Zero;
}

/* キーボード入力 */
bool InputAction::IsKeyDown(SDL_Scancode key) const {
	return _inputSystem.GetState().keyboard.GetKeyState(key) == BUTTON_STATE::DOWN;

}

bool InputAction::IsKeyUp(SDL_Scancode key) const {
	return _inputSystem.GetState().keyboard.GetKeyState(key) == BUTTON_STATE::UP;
}

bool InputAction::IsKey(SDL_Scancode key) const {
	
	return _inputSystem.GetState().keyboard.GetKeyState(key) == BUTTON_STATE::PRESS;
}

bool InputAction::IsAllKey() const {
	return _inputSystem.GetState().keyboard.IsAllKey();
}

/* マウス入力 */
bool InputAction::IsMouseButtonDown(GameTypes::SDL_MouseButton button) const {
	return _inputSystem.GetState().mouse.GetButtonState(button) == BUTTON_STATE::DOWN;
}

bool InputAction::IsMouseButtonUp(GameTypes::SDL_MouseButton button) const {
	
	return _inputSystem.GetState().mouse.GetButtonState(button) == BUTTON_STATE::UP;
}

bool InputAction::IsMouseButton(GameTypes::SDL_MouseButton button) const {
	
	return _inputSystem.GetState().mouse.GetButtonState(button) == BUTTON_STATE::PRESS;
}

const Vector2& InputAction::GetMousePosition() const {
	return _inputSystem.GetState().mouse.GetPosition();
}

void InputAction::SetRelativeMouseMode(bool isRelative) {
	SDL_SetWindowRelativeMouseMode(_inputSystem.GetWindow(), isRelative);
	_inputSystem.SetRelativeMouseMode(isRelative);
}

/* ゲームパッド入力 */
bool InputAction::IsGamepadButtonDown(SDL_GamepadButton button) const {
	return _inputSystem.GetState().gamepad.GetButtonState(button) == BUTTON_STATE::DOWN;
}

bool InputAction::IsGamepadButtonUp(SDL_GamepadButton button) const {
	
	return _inputSystem.GetState().gamepad.GetButtonState(button) == BUTTON_STATE::UP;
}

bool InputAction::IsGamepadButton(SDL_GamepadButton button) const {
	
	return _inputSystem.GetState().gamepad.GetButtonState(button) == BUTTON_STATE::PRESS;

}

bool InputAction::IsAllGamepadButton() const {
	return _inputSystem.GetState().gamepad.IsAllButton();
}

float InputAction::GetGamepadAxis(SDL_GamepadAxis axis) const {
	return _inputSystem.GetState().gamepad.GetAxis(axis);
}

Vector2 InputAction::GetGamepadStickAxis(GameTypes::SDL_GamepadStick stick) const {
	return _inputSystem.GetState().gamepad.GetStickAxis(stick);
}

#pragma endregion

#pragma region プライベートメソッド



#pragma endregion