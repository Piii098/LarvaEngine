#include "Components/SpriteComponent.h"
#include "GameObjects/GameObject.h"
#include "Game.h"
#include "Math.h"
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
#include "GameObjects/Camera.h"
#include "Singletons/ShaderManager.h"
#include "Renderer.h"

#pragma region �X�v���C�g�Ǘ��N���X()

SpriteComponent::SpriteManager::SpriteManager() {

}


void SpriteComponent::SpriteManager::Load(const std::string& fileName) {
	Texture* tex = nullptr;
	auto iter = _textures.find(fileName);// �w�薼�̗v�f��T���A�����Ȃ����_textures.end()��Ԃ�
	if (iter == _textures.end())  //�L�[�����݂��Ȃ��Ȃ�
	{
		/*�A�z�z��_texture�Ƀt�@�C�����ƃf�[�^��ǉ�*/
		
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

#pragma region �X�v���C�g�`�ʃN���X



#pragma endregion


#pragma region �R���X�g���N�^�F�f�X�g���N�^

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

#pragma region �p�u���b�N�֐�

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

#pragma region �v���C�x�[�g�֐�


#pragma endregion

