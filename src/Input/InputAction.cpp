#include <SDL3/SDL.h>
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
	// 実装を追加
}

bool InputAction::IsAction(const std::string& name, int player) const {
	// 実装を追加
	return false;
}

bool InputAction::IsActionDown(const std::string& name, int player) const {
	// 実装を追加
	return false;
}

bool InputAction::IsActionUp(const std::string& name, int player) const {
	// 実装を追加
	return false;
}

float InputAction::GetActionValue(const std::string& name, int player) const {
	// 実装を追加
	return 0.0f;
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

float InputAction::GetGamepadAxis(SDL_GamepadAxis axis) const {
	return _inputSystem.GetState().gamepad.GetAxis(axis);
}

Vector2 InputAction::GetGamepadStickAxis(GameTypes::SDL_GamepadStick stick) const {
	return _inputSystem.GetState().gamepad.GetStickAxis(stick);
}

#pragma endregion

#pragma region プライベートメソッド



#pragma endregion