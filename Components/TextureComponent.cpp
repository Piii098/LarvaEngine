#include "Components/TextureComponent.h"
#include "GameObjects/GameObject.h"
#include "Game.h"
#include "Math.h"
#include "Utilities/Shader.h"
#include "Renderer.h"

#pragma region スプライト管理クラス()

TextureComponent::TextureManager::TextureManager() {

}


void TextureComponent::TextureManager::Load(const std::string& fileName) {
	Texture* tex = nullptr;
	auto iter = _textures.find(fileName);// 指定名の要素を探索、もしなければ_textures.end()を返す
	if (iter == _textures.end())  //キーが存在しないなら
	{
		/*連想配列_textureにファイル名とデータを追加*/

		tex = new Texture();

		if (tex->Load(fileName))
		{
			_textures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}

	}

}

void TextureComponent::TextureManager::Unload() {
	for (auto& pair : _textures) {
		pair.second->Unload();
		delete pair.second;
	}
	_textures.clear();
}

Texture* TextureComponent::TextureManager::GetTexture(const std::string& textureName) {
	return _textures[textureName];
}

#pragma endregion

#pragma region コンストラクタ:デストラクタ

TextureComponent::TextureManager TextureComponent::s_TextureManager;

TextureComponent::TextureComponent(GameObject* parent, int drawLayer)
	: Component(parent, drawLayer)
	, _texture(nullptr)
	, _texWidth(0.f)
	, _texHeight(0.f)
	, _selfLightColor(Vector3(1.f,1.f,1.f))
	, _selfLightIntensity(0.f){

}

TextureComponent::~TextureComponent() {
	delete _texture;
	_texture = nullptr;
}

#pragma endregion

void TextureComponent::Draw(Shader* shader) {
    if (_texture) {

		float scaleX = _flipX ? -1.f : 1.f;
		float scaleY = _flipY ? -1.f : 1.f;

		Matrix4 scaleMat = Matrix4::CreateScale(scaleX * static_cast<float>(_texWidth), scaleY * static_cast<float>(_texHeight), 1.0f);
		Matrix4 world = scaleMat * _parent->WorldTransform();

        shader->SetMatrixUniform("uWorldTransform", world);
        shader->SetVector2Uniform("uTexOffset", Vector2(0.0f, 0.0f));
        shader->SetVector2Uniform("uTexScale", Vector2(1.0f, 1.0f));

        _texture->SetActive();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void TextureComponent::SetTexture(const std::string& textureName) {
    _texture = new Texture();
    if (_texture->Load(textureName)) {
        _texWidth = _texture->Width();
        _texHeight = _texture->Height();
    }
}