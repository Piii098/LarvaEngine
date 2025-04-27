#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/GameObject.h"


class SpriteComponent;
class TextComponent;
class ButtonComponent;

class ButtonGroup {
public:
	enum class INPUT_STATE {
		MOUSE,
		KEY_BUTTON,
	};

	ButtonGroup();
	~ButtonGroup();

	void AddButton(ButtonComponent* button);

	void InputProcess(const InputAction& input);

	void InputKeyAndButton(const InputAction& input);

	void InputMouse(const InputAction& input);

	void MoveFocus(const InputAction& input);

	void ConfirmSelection(const InputAction& input);
	
	int CheckHover();

	void SetInitialSelection(int index);
private:
	std::vector<ButtonComponent*> _buttons;
	int _currentSelection;
	INPUT_STATE _inputState;

	void UpdateSelection();
};

class Button : public GameObject {
public:
	Button(Scene& scene, const std::string& spriteName);
	~Button() override;

	void AddToGroup(ButtonGroup& group);

	void SetText(const std::string& text, const Vector3& color = Vector3(1,1,1), int pointSize = 24);

	void SetOnClick(std::function<void()> callback);
	void SetOnHover(std::function<void()> callback);

private:
	SpriteComponent* _spriteComponent;
	TextComponent* _textComponent;
	ButtonComponent* _buttonComponent;

};