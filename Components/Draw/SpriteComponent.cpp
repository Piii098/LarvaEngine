#include "Components/Draw/SpriteComponent.h"
#include "GameObjects/GameObject.h"
#include "Core/Game.h"
#include "Utilities/Math.h"
#include "Renderer/Shader.h"
#include "AssetManagers/AssetData/Texture.h"
#include "GameObjects/Camera.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#pragma region コンストラクタ：デストラクタ

SpriteComponent::SpriteComponent(GameObject* parent, int drawLayer)
	: TextureComponent(parent, drawLayer){
	_parent->GetScene()->GetManager()->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	_parent->GetScene()->GetManager()->GetGame()->GetRenderer()->RemoveSprite(this);
}

#pragma endregion

#pragma region パブリック関数

void SpriteComponent::Render(Shader* shader) {

	if (_texture) {


		float scaleX = _flipX ? -1.f : 1.f;
		float scaleY = _flipY ? -1.f : 1.f;

		Matrix4 scaleMat = Matrix4::CreateScale( scaleX * static_cast<float>(_texWidth) * _texScale.x, scaleY * static_cast<float>(_texWidth) * _texScale.x, 1.0f);
		Matrix4 world = scaleMat * _parent->WorldTransform();

		shader->SetMatrixUniform("uWorldTransform", world);
		shader->SetVector2Uniform("uTexOffset", _texOffset);
        shader->SetVector2Uniform("uTexScale", _texScale);
		shader->SetVector3Uniform("selfLightColor", _selfLightColor);
		shader->SetFloatUniform("selfLightIntensity", _selfLightIntensity);

		_texture->SetActive();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}

#pragma endregion

#pragma region プライベート関数


#pragma endregion

