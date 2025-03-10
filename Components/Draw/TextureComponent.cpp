#include "Components/Draw/TextureComponent.h"
#include "GameObjects/GameObject.h"
#include "Core/Game.h"
#include "Utilities/Math.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "AssetManagers/AssetManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#pragma region コンストラクタ:デストラクタ

TextureComponent::TextureComponent(GameObject* parent, int bufferLayer, int drawLayer)
	: Component(parent)
	, _texture(nullptr)
	, _texWidth(0.f)
	, _texHeight(0.f)
	, _texOffset(Vector2::Zero)
	, _texScale(Vector2(1.f, 1.f))
	, _selfLightColor(Vector3(1.f,1.f,1.f))
	, _selfLightIntensity(0.f)
	, _flipX(false)
	, _flipY(false)
	, _drawLayer(drawLayer)
	, _bufferLayer(bufferLayer){
	_textureManager = _parent->GetScene()->GetManager()->GetGame()->GetTextureManager();
}

TextureComponent::~TextureComponent() {
	delete _texture;
	_texture = nullptr;
}

#pragma endregion

void TextureComponent::Render(Shader* shader) {
    if (_texture) {

		float scaleX = _flipX ? -1.f : 1.f;
		float scaleY = _flipY ? -1.f : 1.f;

		Matrix4 scaleMat = Matrix4::CreateScale(25, 25, 1.0f);
		Matrix4 world = scaleMat ;

        shader->SetMatrixUniform("uWorldTransform", world);
        shader->SetVector2Uniform("uTexOffset", _texOffset);
        shader->SetVector2Uniform("uTexScale", _texScale);

        _texture->SetActive();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void TextureComponent::SetTexture(const std::string& textureName) {
	_texture = _textureManager->Get(textureName);
	if (_texture) {
		_texWidth = _texture->Width();
		_texHeight = _texture->Height();
	}
	else {
		SDL_Log("Failed to load texture: %s", textureName.c_str());
	}
}