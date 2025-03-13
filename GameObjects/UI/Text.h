#pragma once
#include <string>
#include "GameObjects/GameObject.h"

class TextComponent;

class Text : public GameObject {
public:
	Text(Scene* scene, const std::string& fontName, const Vector3& color, int pointSize, const std::string& text);
	
	~Text() override;

	void CreateOutline(const Vector3& color);

private:

	TextComponent* _textComp;
	TextComponent* _outlineComp;
};