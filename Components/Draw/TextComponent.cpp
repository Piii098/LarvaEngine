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
	: TextureComponent(gameObject, bufferLayer)
	, _textTexture(nullptr) {
	_font = GetParent()->GetScene()->GetManager()->GetGame()->GetFontManager()->Get(fontName);

	GetParent()->GetScene()->GetManager()->GetGame()->GetRenderer()->AddText(this);
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

void TextComponent::CreateTextTexture(std::string text, Vector3 color, int pointSize) {
	if (_texture) {
		_texture->Unload();
		delete _texture;
		_texture = nullptr;
	}

	_texture = _font->RenderText(text, color, pointSize);

	if (_texture) {
		_texWidth = _texture->Width();
		_texHeight = _texture->Height();
	}
	else {
		SDL_Log("Failed to load texture: %s", text.c_str());
	}

}

void TextComponent::RenderText(Shader* shader, const Vector2Int& offset, float scale, Vector3 color) {
	
	if (!_texture) {
		return;
	}

	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(_texWidth), static_cast<float>(_texHeight), 1.0f);
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));
	Matrix4 world = scaleMat * GetParent()->WorldTransform();

	shader->SetMatrixUniform("uWorldTransform", world);
	shader->SetVector2Uniform("uTexOffset", Vector2(1.f, 1.f));
	shader->SetVector2Uniform("uTexScale", Vector2(1.f, 1.f));
	shader->SetVector3Uniform("selfLightColor", Vector3(1.f, 1.f, 1.f));
	shader->SetFloatUniform("selfLightIntensity", 1.f);

	_texture->SetActive();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}


#pragma endregion
