#include "LarvaEngine/Components/Draw/TextComponent.h"
#include "LarvaEngine/Core/Resources/Font.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"

#pragma region コンストラクタデストラクタ

TextComponent::TextComponent(GameObject& parent, int bufferLayer, std::string fontName)
	: SpriteComponent(parent, bufferLayer) {
	_font = _parent.GetScene().GetManager().GetGame().GetFontManager().Get(fontName);

	_fontName = fontName;
	//GetParent()->GetScene()->GetManager()->GetGame()->GetRenderer()->AddText(this);
}

TextComponent::~TextComponent() {

}

#pragma endregion

#pragma region パブリック関数

void TextComponent::CreateTextTexture(std::string text, Vector3 color, int pointSize) {


	_text = text;
	_pointSize = pointSize;
	_textColor = color;
	_texture = _font->RenderTextCached(text, color, pointSize);

	if (_texture) {
		_texWidth = _texture->Width();
		_texHeight = _texture->Height();
	}
	else {
		SDL_Log("Failed to load texture: %s", text.c_str());
	}

}


#pragma endregion
