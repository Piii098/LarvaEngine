#include "Utilities/Input.h"

#pragma region インプット管理クラス

#pragma region コンストラクタ:デストラクタ

Input::InputManager::InputManager()
	: _currentKeyState{}
	, _previousKeyState{}
	, _keyDownState{} {
	_keyboradMap[InputMap::INPUT_DUP] = SDL_SCANCODE_UP;
	_keyboradMap[InputMap::INPUT_DDOWN] = SDL_SCANCODE_DOWN;
	_keyboradMap[InputMap::INPUT_DLEFT] = SDL_SCANCODE_LEFT;
	_keyboradMap[InputMap::INPUT_DRIGHT] = SDL_SCANCODE_RIGHT;
	_keyboradMap[InputMap::INPUT_BUP] = SDL_SCANCODE_W;
	_keyboradMap[InputMap::INPUT_BDOWN] = SDL_SCANCODE_S;
	_keyboradMap[InputMap::INPUT_BLEFT] = SDL_SCANCODE_A;
	_keyboradMap[InputMap::INPUT_BRIGHT] = SDL_SCANCODE_D;
	_keyboradMap[InputMap::INPUT_START] = SDL_SCANCODE_TAB;
	_keyboradMap[InputMap::INPUT_BACK] = SDL_SCANCODE_ESCAPE;

}

Input::InputManager::~InputManager() {

}
#pragma endregion

#pragma region パブリック関数

void Input::InputManager::Update() {
	UpdateKeyboard();
}

bool Input::InputManager::IsKeyDown(InputMap input) {
	return _keyDownState[GetKeyCode(input)];
}

bool Input::InputManager::IsKey(InputMap input) {
	return _currentKeyState[GetKeyCode(input)];
}

void Input::InputManager::UpdateKeyboard() {
	const bool* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < SDL_SCANCODE_COUNT;i++) {
		_currentKeyState[i] = keys[i];
	}

	for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
		_keyDownState[i] = (_currentKeyState[i] != 0) && (_previousKeyState[i] == 0);
	}

	for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
		_previousKeyState[i] = _currentKeyState[i];
	}

}

#pragma endregion

#pragma region プライベート関数

int Input::InputManager::GetKeyCode(InputMap input) {
	return _keyboradMap[input];
}

#pragma endregion

#pragma endregion

#pragma region コンストラクタ:デストラクタ

Input::InputManager Input::s_InputManager;

Input::Input() {
	
}

Input::~Input() {

}

#pragma endregion

#pragma region パブリック関数

void Input::Update() {
	s_InputManager.Update();
}

bool Input::IsInputDown(InputMap key) {
	return s_InputManager.IsKeyDown(key);
}

bool Input::IsInput(InputMap key) {
	return s_InputManager.IsKey(key);
}

bool Input::CheckInputAll() {
	return (CheckHitKeyboradAll());
}

#pragma endregion

#pragma region プレイべーと関数

bool Input::CheckHitKeyboradAll() {
	const bool* keys = SDL_GetKeyboardState(NULL);


	for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
		if (keys[i]) {
		
			return true;
		}
	}

	return false;
}


#pragma endregion

