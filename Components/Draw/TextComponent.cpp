#include "Components/Draw/TextComponent.h"
#include "Utilities/Font.h"
#include "AssetManagers/AssetData/Texture.h"
#include "Renderer/Shader.h"
#include "GameObjects/GameObject.h"
#include "Core/Game.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "AssetManagers/AssetManager.h"

#pragma region コンストラクタデストラクタ

TextComponent::TextComponent(GameObject* gameObject, int bufferLayer, std::string fontName)
	: SpriteComponent(gameObject, bufferLayer)
	, _textTexture(nullptr) {
	_font = GetParent()->GetScene()->GetManager()->GetGame()->GetFontManager()->Get(fontName);

	_fontName = fontName;
	//GetParent()->GetScene()->GetManager()->GetGame()->GetRenderer()->AddText(this);
}

TextComponent::~TextComponent() {
	if (_texture) {
		_texture->Unload();
		delete _texture;
		_texture = nullptr;
	}
}

#pragma endregion

#pragma region パブリック関数

void TextComponent::CreateTextTexture(std::string text, Vector3 color, int pointSize, bool isOutline) {
	if (_texture) {
		_texture->Unload();
		delete _texture;
		_texture = nullptr;
	}

	_text = text;
	_pointSize = pointSize;
	_textColor = color;
	_font->IsOutline(isOutline);
	_texture = _font->RenderText(text, color, pointSize);

	if (_texture) {
		_texWidth = _texture->Width();
		_texHeight = _texture->Height();
	}
	else {
		SDL_Log("Failed to load texture: %s", text.c_str());
	}

}


#pragma endregion
