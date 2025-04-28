#include <memory.h>
#include <algorithm>
#include "LarvaEngine/Input/InputSystem.h"
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Renderer/Renderer.h"

#pragma region KeyboardState

BUTTON_STATE KeyboardState::GetKeyState(SDL_Scancode key) const {

	if (_currentKeyState[key] == true && _previousKeyState[key] == false) {
		return BUTTON_STATE::DOWN;
	}
	else if (_currentKeyState[key] == false && _previousKeyState[key] == true) {
		return BUTTON_STATE::UP;
	}
	else if (_currentKeyState[key] == true && _previousKeyState[key] == true) {
		return BUTTON_STATE::PRESS;
	}
	else {
		return BUTTON_STATE::NONE;
	}

}

bool KeyboardState::IsAllKey() const {

	for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
		if (GetKeyState(static_cast<SDL_Scancode>(i)) == BUTTON_STATE::DOWN) {
			return true;
		}
	}

	return false;
}

void KeyboardState::UpdateKeyState(SDL_Scancode key, bool isDown) {

}

#pragma endregion

#pragma region MouseState

bool IsButtonDown(Uint32 state, Uint32 mask) { // マウスボタンが押されているかどうかを判定する関数
	return (state & mask) == mask;
}

BUTTON_STATE MouseState::GetButtonState(GameTypes::SDL_MouseButton button) const { 
	Uint32 mask = GetMouseButtonMask(button); 
	bool isCurrentDown = IsButtonDown(_currentButtons, mask);
	bool isPreviousDown = IsButtonDown(_previousButtons, mask);

	if (isCurrentDown && !isPreviousDown) {
		return BUTTON_STATE::DOWN;      // 押された瞬間
	}
	else if (!isCurrentDown && isPreviousDown) {
		return BUTTON_STATE::UP;        // 離された瞬間
	}
	else if (isCurrentDown && isPreviousDown) {
		return BUTTON_STATE::PRESS;     // 押し続けている
	}
	else {
		return BUTTON_STATE::NONE;      // 押されていない
	}
}

#pragma endregion

#pragma region GamepadState

BUTTON_STATE GamepadState::GetButtonState(SDL_GamepadButton button) const {
	if (_currentButtons[button] == true && _previousButtons[button] == false) {
		return BUTTON_STATE::DOWN;
	}
	else if (_currentButtons[button] == false && _previousButtons[button] == true) {
		return BUTTON_STATE::UP;
	}
	else if (_currentButtons[button] == true && _previousButtons[button] == true) {
		return BUTTON_STATE::PRESS;
	}
	else {
		return BUTTON_STATE::NONE;
	}
}

bool GamepadState::IsAllButton() const {
	for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++) {
		if (GetButtonState(static_cast<SDL_GamepadButton>(i)) == BUTTON_STATE::DOWN) {
			return true;
		}
	}

	return false;
}

float GamepadState::GetAxis(SDL_GamepadAxis axis) const {
	switch (axis) {
	case SDL_GAMEPAD_AXIS_LEFTX:
		return _leftStick.x;
	case SDL_GAMEPAD_AXIS_LEFTY:
		return _leftStick.y;
	case SDL_GAMEPAD_AXIS_RIGHTX:
		return _rightStick.x;
	case SDL_GAMEPAD_AXIS_RIGHTY:
		return _rightStick.y;
	case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
		return _leftTrigger;
	case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
		return _rightTrigger;
	default:
		return 0.f;
	}
}	

Vector2 GamepadState::GetStickAxis(GameTypes::SDL_GamepadStick stick) const {
	switch (stick) {
	case GameTypes::SDL_GamepadStick::SDL_GAMEPAD_LEFT_STICK:
		return _leftStick;
	case GameTypes::SDL_GamepadStick::SDL_GAMEPAD_RIGHT_STICK:
		return _rightStick;
	default:
		return Vector2::Zero;
	}
}

#pragma endregion	

#pragma region コンストラクタ・デストラクタ

InputSystem::InputSystem(Game& game)
	: _game(game), _window(_game.GetRenderer().GetWindow()){

}

InputSystem::~InputSystem() {
}

#pragma endregion

#pragma region パブリックメソッド

bool InputSystem::Initialize() {
	// キーボードの状態を初期化
	const bool* keyboardState = SDL_GetKeyboardState(NULL);
	memcpy(_state.keyboard._currentKeyState, keyboardState, SDL_SCANCODE_COUNT);
	memset(_state.keyboard._previousKeyState, 0, SDL_SCANCODE_COUNT);

	// マウスの状態を初期化
	_state.mouse._currentButtons = 0;
	_state.mouse._previousButtons = 0;

	// コントローラの状態を初期化
	// マッピングファイルを読み込む（エラーでも続行）
	if (SDL_AddGamepadMappingsFromFile("Assets/Etc/gamecontrollerdb.txt") < 0) {
		SDL_Log("マッピングファイルの読み込みに失敗しました（無視して続行）: %s", SDL_GetError());
		// マッピングエラーでも初期化は続行
	}

	return true;
}

void InputSystem::ProcessInput(SDL_Event& event) {
	
	switch (event.type) {
		case SDL_EVENT_MOUSE_WHEEL:
			_state.mouse._wheel = Vector2(event.wheel.x, event.wheel.y);
			break;
		case SDL_EVENT_GAMEPAD_ADDED:
			_gamepad = SDL_OpenGamepad(event.cdevice.which);
			if (_gamepad) {
				SDL_Log("ゲームパッド \"%s\" を接続しました", SDL_GetGamepadName(_gamepad));
				_state.gamepad._isConnected = true;
			}
			break;
		case SDL_EVENT_GAMEPAD_REMOVED:
			SDL_CloseGamepad(_gamepad);
			_gamepad = nullptr;
			SDL_Log("ゲームパッドを切断しました");
			_state.gamepad._isConnected = false;
			break;
		default:
			break;
	}

}

void InputSystem::PreUpdate() {

	// キーボードの状態をコピー
	memcpy(_state.keyboard._previousKeyState, _state.keyboard._currentKeyState, SDL_SCANCODE_COUNT);

	// マウスの状態をコピー
	_state.mouse._previousButtons = _state.mouse._currentButtons;
	_state.mouse._wheel = Vector2::Zero; // ホイールの値をリセット
	_state.mouse._isRelative = false; // マウスの相対モードをリセット

	// コントローラの状態をコピー
	memcpy(_state.gamepad._previousButtons, _state.gamepad._currentButtons, SDL_GAMEPAD_BUTTON_COUNT);

}

// キーボードの状態を更新
void InputSystem::KeyboardUpdate() { // ヘルパー関数 
	const bool* keyboardState = SDL_GetKeyboardState(NULL);

	if (keyboardState) {
		memcpy(_state.keyboard._currentKeyState, keyboardState, SDL_SCANCODE_COUNT);
	}
}

// コントローラの状態を更新
void InputSystem::GamepadUpdate() {
	if (_gamepad == nullptr) return;
	for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++) {
		_state.gamepad._currentButtons[i] = SDL_GetGamepadButton(_gamepad, (SDL_GamepadButton)i);
	}

	_state.gamepad._leftTrigger = Filter1D(SDL_GetGamepadAxis(_gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER));
	_state.gamepad._rightTrigger = Filter1D(SDL_GetGamepadAxis(_gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER));
	
	_state.gamepad._leftStick = Filter2D(SDL_GetGamepadAxis(_gamepad, SDL_GAMEPAD_AXIS_LEFTX), SDL_GetGamepadAxis(_gamepad, SDL_GAMEPAD_AXIS_LEFTY));
	_state.gamepad._rightStick = Filter2D(SDL_GetGamepadAxis(_gamepad, SDL_GAMEPAD_AXIS_RIGHTX), SDL_GetGamepadAxis(_gamepad, SDL_GAMEPAD_AXIS_RIGHTY));
}

// マウスの状態を更新
void InputSystem::MouseUpdate() {// ヘルパー関数
	float x = 0;
	float y = 0;
	if (_state.mouse._isRelative) {
		_state.mouse._currentButtons = SDL_GetRelativeMouseState(&x, &y);
	}
	else {
		_state.mouse._currentButtons = SDL_GetMouseState(&x, &y);
	}
	_state.mouse._position = Vector2(x, y);

}

void InputSystem::AxisUpdate(float deltaTime) {
	// 各軸のマッピングに対して値を更新
	for (auto& pair : _axisMap) {
		AxisMapping& mapping = pair.second;
		float targetValue = 0.0f;

		// positiveの入力がキーボードの場合
		if (std::holds_alternative<SDL_Scancode>(mapping.positive)) {
			SDL_Scancode key = std::get<SDL_Scancode>(mapping.positive);
			if (_state.keyboard.GetKeyState(key) == BUTTON_STATE::PRESS ||
				_state.keyboard.GetKeyState(key) == BUTTON_STATE::DOWN) {
				targetValue += 1.0f;
			}
		}
		// positiveの入力がゲームパッドボタンの場合
		else if (std::holds_alternative<SDL_GamepadButton>(mapping.positive)) {
			SDL_GamepadButton button = std::get<SDL_GamepadButton>(mapping.positive);
			if (_state.gamepad.GetButtonState(button) == BUTTON_STATE::PRESS ||
				_state.gamepad.GetButtonState(button) == BUTTON_STATE::DOWN) {
				targetValue += 1.0f;
			}
		}
		// positiveの入力がマウスボタンの場合
		else if (std::holds_alternative<GameTypes::SDL_MouseButton>(mapping.positive)) {
			GameTypes::SDL_MouseButton button = std::get<GameTypes::SDL_MouseButton>(mapping.positive);
			if (_state.mouse.GetButtonState(button) == BUTTON_STATE::PRESS ||
				_state.mouse.GetButtonState(button) == BUTTON_STATE::DOWN) {
				targetValue += 1.0f;
			}
		}

		// negativeも同様に処理
		if (std::holds_alternative<SDL_Scancode>(mapping.negative)) {
			SDL_Scancode key = std::get<SDL_Scancode>(mapping.negative);
			if (_state.keyboard.GetKeyState(key) == BUTTON_STATE::PRESS ||
				_state.keyboard.GetKeyState(key) == BUTTON_STATE::DOWN) {
				targetValue -= 1.0f;
			}
		}
		else if (std::holds_alternative<SDL_GamepadButton>(mapping.negative)) {
			SDL_GamepadButton button = std::get<SDL_GamepadButton>(mapping.negative);
			if (_state.gamepad.GetButtonState(button) == BUTTON_STATE::PRESS ||
				_state.gamepad.GetButtonState(button) == BUTTON_STATE::DOWN) {
				targetValue -= 1.0f;
			}
		}
		else if (std::holds_alternative<GameTypes::SDL_MouseButton>(mapping.negative)) {
			GameTypes::SDL_MouseButton button = std::get<GameTypes::SDL_MouseButton>(mapping.negative);
			if (_state.mouse.GetButtonState(button) == BUTTON_STATE::PRESS ||
				_state.mouse.GetButtonState(button) == BUTTON_STATE::DOWN) {
				targetValue -= 1.0f;
			}
		}
		
		// 現在の値を更新（重力とsensitivityを考慮）
		float currentValue = mapping.value;


		// 入力がない場合は重力で0に戻す
		if (targetValue == 0.0f) {
			if (currentValue > 0.0f) {
				currentValue -= mapping.gravity * deltaTime;
				if (currentValue < 0.0f) currentValue = 0.0f;
			}
			else if (currentValue < 0.0f) {
				currentValue += mapping.gravity * deltaTime;
				if (currentValue > 0.0f) currentValue = 0.0f;
			}
		}
		// 入力がある場合は感度に従って目標値に近づける
		else {
			if (mapping.snap && Math::Sign(currentValue) != Math::Sign(targetValue)) {
				currentValue = 0.0f;
			}

			float delta = mapping.sensitivity * deltaTime;
			if (currentValue < targetValue) {
				currentValue += delta;
				if (currentValue > targetValue) currentValue = targetValue;
			}
			else if (currentValue > targetValue) {
				currentValue -= delta;
				if (currentValue < targetValue) currentValue = targetValue;
			}
		}

		// デッドゾーンを適用
		if (Math::Abs(currentValue) < mapping.deadZone) {
			currentValue = 0.0f;
		}
		
		mapping.value = currentValue;
	}
}

float InputSystem::GetAxis(const std::string& name) const {
	auto it = _axisMap.find(name);
	if (it != _axisMap.end()) {
		return it->second.value;
	}
	return 0.0f; // デフォルト値を返す（必要に応じて変更）
}

void InputSystem::Update(float deltaTime) {

	// キーボードの状態を更新
	KeyboardUpdate();

	// コントローラの状態を更新
	GamepadUpdate();

	// マウスの状態を更新
	MouseUpdate();

	AxisUpdate(deltaTime);
}

void InputSystem::Shutdown() {

}

void InputSystem::SetAxisMapping(const std::string& name, GameTypes::InputCode negative, GameTypes::InputCode positive) {
	AxisMapping mapping;
	mapping.positive = positive;
	mapping.negative = negative;

	_axisMap[name] = mapping;
}

void InputSystem::SetRelativeMouseMode(bool isRelative) {
	_state.mouse._isRelative = isRelative;
}

#pragma endregion

#pragma region プライベートメソッド

float InputSystem::Filter1D(int input) {
	const int deadZone = 250;
	const int maxValue = 30000;

	float value = 0.f;

	int adsValue = Math::Abs(input);
	
	if (adsValue > deadZone) { // デッドゾーンを超えている

		value = static_cast<float>(adsValue - deadZone) / (maxValue - deadZone);

		value = input > 0 ? value : -value;

		value = Math::Clamp(value, -1.f, 1.f);

	}

	return value;
}

Vector2 InputSystem::Filter2D(int x, int y) {
	const int deadZone = 8000;
	const int maxValue = 30000;

	Vector2 dir;
	dir.x = static_cast<float>(x);
	dir.y = static_cast<float>(y);

	float length = dir.Length();

	if (length < deadZone) {
		dir = Vector2::Zero;
	}
	else {
		float f = (length - deadZone) / (maxValue - deadZone);
		f = Math::Clamp(f, 0.f, 1.f);
		dir *= f / length;
	}

	return dir;
}

#pragma endregion