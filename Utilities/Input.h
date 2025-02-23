#pragma once
#include <unordered_map>
#include <SDL3/SDL.h>

enum class InputMap {
	INPUT_DUP, //十字キー
	INPUT_DDOWN,
	INPUT_DLEFT,
	INPUT_DRIGHT,
	INPUT_BUP, //丸キー上
	INPUT_BDOWN, //丸キー下
	INPUT_BLEFT, //丸キー左
	INPUT_BRIGHT, //丸キー右
	INPUT_START,
	INPUT_BACK
};

class Input {
public: 
	
	#pragma region インプット管理クラス

	class InputManager {
	public:
		InputManager();
		~InputManager();

		void Update(); //全キーチェックアンド記録
		void UpdateKeyboard(); // キーボードの状態を更新
		bool IsKeyDown(InputMap input); //押した瞬間のみ
		bool IsKey(InputMap input);

	private:

		int GetKeyCode(InputMap);

		bool _currentKeyState[SDL_SCANCODE_COUNT];
		bool _keyDownState[SDL_SCANCODE_COUNT];
		bool _previousKeyState[SDL_SCANCODE_COUNT];

		std::unordered_map<InputMap, int> _keyboradMap;
	};

	#pragma endregion

	Input();
	~Input();

	void Update();
	bool IsInputDown(InputMap input); //押した瞬間trueを返す
	bool IsInput(InputMap input); //押している間trueを返す

	static InputManager& S_InputManager() { return s_InputManager; };
	bool CheckInputAll();
private:
	static InputManager s_InputManager;

	bool CheckHitKeyboradAll();
};