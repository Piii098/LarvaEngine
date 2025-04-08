
/**
* @file FileName.h
* @brief 入力アクションを管理するクラス
*
* 
*/

#pragma once
#include <vector>
#include <string>
#include <SDL3/SDL_keycode.h>
#include <variant>
#include <memory>
#include "LarvaEngine/Core/Utilities/DataTypes.h"
#include "LarvaEngine/Core/Utilities/Math.h"

class InputSystem;

class InputAction {
public:

	InputAction(InputSystem& inputSystem);
	~InputAction();

	void MapAction(const std::string& name, GameTypes::InputCode code, int playerIndex = 0); // 入力アクションを登録
	
	bool IsAction(const std::string& name, int player = 0) const; // 入力があるかどうか
	bool IsActionDown(const std::string& name, int player = 0) const; // 入力があった瞬間かどうか
	bool IsActionUp(const std::string& name, int player = 0) const; // 入力が離された瞬間かどうか
	
	float GetActionValue(const std::string& name, int player = 0) const; // 入力の値を取得
	Vector2 GetActionVector(const std::string& name, int player = 0) const; // 入力のベクトルを取得

	bool IsKeyDown(SDL_Scancode key) const;
	bool IsKeyUp(SDL_Scancode key) const;
	bool IsKey(SDL_Scancode key) const;

	bool IsMouseButtonDown(GameTypes::SDL_MouseButton button) const;
	bool IsMouseButtonUp(GameTypes::SDL_MouseButton button) const;
	bool IsMouseButton(GameTypes::SDL_MouseButton button) const;
	const Vector2& GetMousePosition() const;
	void SetRelativeMouseMode(bool flag);

	bool IsGamepadButtonDown(SDL_GamepadButton button) const;
	bool IsGamepadButtonUp(SDL_GamepadButton button) const;
	bool IsGamepadButton(SDL_GamepadButton button) const;
	float GetGamepadAxis(SDL_GamepadAxis axis) const;
	Vector2 GetGamepadStickAxis(GameTypes::SDL_GamepadStick stick) const;

private:
	InputSystem& _inputSystem;
};
