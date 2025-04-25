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

class Button : public GameObject {
public:
	Button(Scene& scene, const std::string& spriteName);
	~Button() override;

	void SetText(const std::string& text);

	void SetOnClick(std::function<void()> callback);
	void SetOnHover(std::function<void()> callback);


private:
	SpriteComponent* _spriteComponent;
	TextComponent* _textComponent;
	ButtonComponent* _buttonComponent;

};