#include "GameObjects/UI/Text.h"
#include "Components/Draw/TextComponent.h"

#pragma region コンストラクタデストラクタ

Text::Text(Scene* scene, const std::string& fontName, const Vector3& color, int pointSize, const std::string& text)
	: GameObject(scene)
{
	CreateText(fontName, color, pointSize, text);
}

Text::~Text() {
	if (_textComp) {
		delete _textComp;
		_textComp = nullptr;
	}
}

#pragma endregion

#pragma region パブリック関数

void Text::CreateOutline(const Vector3& color) {
	if (_outlineComp) {
		delete _outlineComp;
		_outlineComp = nullptr;
	}

	_outlineComp = new TextComponent(this, 0, _textComp->GetFontName());
	_outlineComp->CreateTextTexture(_textComp->GetText(), color, _textComp->GetPointSize(), true);

}

void Text::CreateText(const std::string& fontName, const Vector3& color, int pointSize, const std::string& text) {
	if (_textComp) {
		_textComp->CreateTextTexture(text, color, pointSize);
	}
	else {
		_textComp = new TextComponent(this, 0, fontName);
		_textComp->CreateTextTexture(text, color, pointSize);
	}
}

void Text::SetAlignLeft() {
	_textComp->SetHorizontalAlign(SpriteComponent::HorizontalAlign::Left);
}


#pragma endregion
