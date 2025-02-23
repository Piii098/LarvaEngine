#pragma once
#include <unordered_map>
#include <SDL3/SDL.h>

enum class InputMap {
	INPUT_DUP, //�\���L�[
	INPUT_DDOWN,
	INPUT_DLEFT,
	INPUT_DRIGHT,
	INPUT_BUP, //�ۃL�[��
	INPUT_BDOWN, //�ۃL�[��
	INPUT_BLEFT, //�ۃL�[��
	INPUT_BRIGHT, //�ۃL�[�E
	INPUT_START,
	INPUT_BACK
};

class Input {
public: 
	
	#pragma region �C���v�b�g�Ǘ��N���X

	class InputManager {
	public:
		InputManager();
		~InputManager();

		void Update(); //�S�L�[�`�F�b�N�A���h�L�^
		void UpdateKeyboard(); // �L�[�{�[�h�̏�Ԃ��X�V
		bool IsKeyDown(InputMap input); //�������u�Ԃ̂�
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
	bool IsInputDown(InputMap input); //�������u��true��Ԃ�
	bool IsInput(InputMap input); //�����Ă����true��Ԃ�

	static InputManager& S_InputManager() { return s_InputManager; };
	bool CheckInputAll();
private:
	static InputManager s_InputManager;

	bool CheckHitKeyboradAll();
};