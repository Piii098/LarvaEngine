#pragma once
#include <vector>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_gamepad.h>
#include <memory>
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/Utilities/DataTypes.h"

// ===== 前方宣言 ===== //
union SDL_Event;
class Game;
class SDL_Window;

/// @brief ボタンの状態
enum class BUTTON_STATE {
	NONE, ///< 何も押されていない
	DOWN, ///< 押された瞬間
	UP,   ///< 離された瞬間
	PRESS ///< 押し続けている
};

/// @brief キーボードの状態
/// キーボードの状態を管理し、InputSystemによって使用される
class KeyboardState {
public:
	// フレンド宣言
	friend class InputSystem;

	// ===== メンバ関数 ===== 

	/// @brief キーの状態を取得する
	/// @param key キーコード
	/// @return キーの状態
	BUTTON_STATE GetKeyState(SDL_Scancode key) const;
	
	/// @brief キーの状態を更新する
	/// @param key キーコード
	/// @param isDown キーが押されているかどうか
	void UpdateKeyState(SDL_Scancode key, bool isDown);

private:
	bool _currentKeyState[SDL_SCANCODE_COUNT];	///< 現在のキーの状態
	bool _previousKeyState[SDL_SCANCODE_COUNT];	///< 1フレーム前のキーの状態
};

/// @brief マウスの状態
/// マウスの状態を管理し、InputSystemによって使用される
class MouseState {
public:
	// フレンド宣言
	friend class InputSystem;

	// ===== メンバ関数 ===== //

	/// @brief マウスの座標を取得する
	/// @return マウスの座標
	const Vector2& GetPosition() const { return _position; }

	/// @brief マウスのボタンの状態を取得する
	/// @param button マウスボタン
	/// @return ボタンの状態
	BUTTON_STATE GetButtonState(GameTypes::SDL_MouseButton button) const;


	bool IsRealtive() const { return _isRelative; }
	void IsRelative(bool flag) { _isRelative = flag; }

private:
	Vector2 _position;			///< マウスの座標
	Vector2 _wheel;				///< マウスのホイール
	Uint32 _currentButtons;		///< 現在のボタンの状態
	Uint32 _previousButtons;	///< 1フレーム前のボタンの状態
	bool _isRelative;			///< 相対座標かどうか
};


/// @brief ゲームパッドの状態
/// ゲームパッドの状態を管理し、InputSystemによって使用される
class GamepadState {
public:
	// フレンド宣言
	friend class InputSystem;

	// ===== メンバ関数 ===== //

	/// @brief ボタンの状態を取得する
	/// @param button ボタン
	/// @return ボタンの状態
	BUTTON_STATE GetButtonState(SDL_GamepadButton button) const;

	bool GetIsConnected() const { return _isConnected; }

	/// @brief トリガー、スティックの1次元軸の値を取得する
	/// @param axis ゲームパッドの軸
	float GetAxis(SDL_GamepadAxis axis) const;

	/// @brief スティックの2次元軸の値を取得する
	/// @param stick ゲームパッドのスティック
	Vector2 GetStickAxis(GameTypes::SDL_GamepadStick stick) const;

private:

	Uint8 _currentButtons[SDL_GAMEPAD_BUTTON_COUNT];
	Uint8 _previousButtons[SDL_GAMEPAD_BUTTON_COUNT];
	float _leftTrigger;
	float _rightTrigger;
	Vector2 _leftStick;
	Vector2 _rightStick;
	bool _isConnected;
};

struct InputState {
	KeyboardState keyboard;
	MouseState mouse;
	GamepadState gamepad;
};

/// @brief 入力システム
/// キーボード、マウス、ゲームパッドの入力を管理、更新する
class InputSystem {
public:


	InputSystem(Game& game);
	~InputSystem();

	bool Initialize();
	void ProcessInput(SDL_Event& event);
	void Update();
	void Shutdown();
	void PreUpdate();

	void SetRelativeMouseMode(bool flag);

	const InputState& GetState() const { return _state; }
	SDL_Window* GetWindow() const { return _window; }
private:
	void KeyboardUpdate();
	void GamepadUpdate();
	void MouseUpdate();


	float Filter1D(int input);
	Vector2 Filter2D(int inputX, int inputY);

	Game& _game;
    SDL_Window* _window;
	InputState _state; // 入力状態
	SDL_Gamepad* _gamepad;

};