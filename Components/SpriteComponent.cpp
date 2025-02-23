#include "Components/SpriteComponent.h"
#include "GameObjects/GameObject.h"
#include "Game.h"
#include "Math.h"
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
#include "GameObjects/Camera.h"
#include "Singletons/ShaderManager.h"
#include "Renderer.h"

#pragma region スプライト管理クラス()

SpriteComponent::SpriteManager::SpriteManager() {

}


void SpriteComponent::SpriteManager::Load(const std::string& fileName) {
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

void SpriteComponent::SpriteManager::Unload() {
	for (auto& pair : _textures) {
		delete pair.second;
	}
	_textures.clear();
}

Texture* SpriteComponent::SpriteManager::GetTexture(const std::string& textureName) {
	return _textures[textureName];
}

#pragma endregion

#pragma region スプライト描写クラス



#pragma endregion


#pragma region コンストラクタ：デストラクタ

SpriteComponent::SpriteManager SpriteComponent::s_SpriteManager;

SpriteComponent::SpriteComponent(GameObject* parent, int drawLayer)
	: Component(parent, drawLayer)
	, _texture(nullptr)
	, _texWidth(0)
	, _texHeight(0){

	_parent->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	_parent->GetGame()->GetRenderer()->RemoveSprite(this);
}

#pragma endregion

#pragma region パブリック関数

void SpriteComponent::Draw(Shader* shader) {

	if (_texture) {

		float scaleX = _flipX ? -1.f : 1.f;
		float scaleY = _flipY ? -1.f : 1.f;

		Matrix4 scaleMat = Matrix4::CreateScale(scaleX * static_cast<float>(_texWidth),  scaleY * static_cast<float>(_texHeight), 1.0f);
		Matrix4 world = scaleMat * _parent->WorldTransform();

		Matrix4 view = _parent->GetGame()->GetCamera()->GetViewMatrix();
		Matrix4 worldViewProj = world * view;

		shader->SetMatrixUniform("uWorldTransform", worldViewProj);
		shader->SetVector2Uniform("uTexOffset", Vector2(0.0f, 0.0f));
		shader->SetVector2Uniform("uTexScale", Vector2(1.0f, 1.0f));


		_texture->SetActive();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}

void SpriteComponent::SetTexture(const std::string& textureName) {
	_texture = s_SpriteManager.GetTexture(textureName);

	_texWidth = _texture->Width();
	_texHeight = _texture->Height();

}

#pragma endregion

#pragma region プライベート関数


#pragma endregion

