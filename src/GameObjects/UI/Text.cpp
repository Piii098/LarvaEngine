#include "LarvaEngine/GameObjects/UI/Text.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"

#pragma region コンストラクタデストラクタ

Text::Text(Scene& scene, const std::string& fontName, const Vector3& color, int pointSize, const std::string& text)
	: GameObject(scene)
{
	CreateText(fontName, color, pointSize, text);
}

Text::~Text() {
	
}

#pragma endregion

#pragma region パブリック関数


void Text::CreateText(const std::string& fontName, const Vector3& color, int pointSize, const std::string& text) {
	if (_textComp) {
		_textComp->CreateTextTexture(text, color, pointSize);
	}
	else {
		CreateComponent<TextComponent>(10, fontName);
		_textComp = GetComponent<TextComponent>();
		_textComp->CreateTextTexture(text, color, pointSize);
	}
}

void Text::SetAlignLeft() {
	_textComp->SetHorizontalAlign(SpriteComponent::HorizontalAlign::Left);
}


#pragma endregion
