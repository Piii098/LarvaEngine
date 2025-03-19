#pragma once
#include <string>
#include "LarvaEngine/Core/GameObject.h"

class TextComponent;

class Text : public GameObject {
public:
	Text(Scene* scene, const std::string& fontName, const Vector3& color, int pointSize, const std::string& text);
	
	~Text() override;

	void CreateOutline(const Vector3& color);

	void SetAlignLeft();

protected:

	void CreateText(const std::string& fontName, const Vector3& color, int pointSize, const std::string& text);

	TextComponent* _textComp;
	TextComponent* _outlineComp;
};