#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include "LarvaEngine/Core/GameObject.h"

class Texture;
class SpriteComponent;
class TextComponent;


struct ButtonData {
	bool isPressed = false;
	bool isHovered = false;
	std::function<void()> onClick = nullptr;
	SpriteComponent* spriteComp = nullptr;
	TextComponent* textComp = nullptr;
};

class Button : public GameObject {
public:
	Button(Scene* scene);
	~Button() override;

	void InputObject(Input* input) override;

	void UpdateObject(float deltaTime) override;
	void SetText(int key, const std::string& text, const std::string& fontName, int pointSize, const Vector3& color);

	void SetOnClick(int key, std::function<void()> onClick);
	
	void CreateButtonData();

	void SetOffset(const Vector2& offset);

private:

	Vector2 _offset;

	bool _isPressed;
	bool _isHovered;
	std::function<void()> _onClick;
	SpriteComponent* _spriteComp;
	TextComponent* _textComp;

	std::vector<ButtonData> _buttonData;
	int _hoveredIndex;
};