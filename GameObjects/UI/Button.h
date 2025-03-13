#pragma once
#include <functional>
#include <string>
#include "GameObjects/GameObject.h"

class Texture;
class SpriteComponent;
class TextComponent;


class Button : public GameObject {
public:
	Button(Scene* scene);
	~Button() override;

	void InputObject(Input* input) override;
	void SetText(const std::string& text, const std::string& fontPath, int pointSize, const Vector3& color);
	void IsPresse(bool isPressed) { _isPressed = isPressed; }
	bool IsPressed() const { return _isPressed; }

	void SetOnClick(std::function<void()> onClick) { _onClick = onClick; }

private:
	bool _isPressed;
	bool _isHovered;
	std::function<void()> _onClick;
	SpriteComponent* _spriteComp;
	TextComponent* _textComp;
};