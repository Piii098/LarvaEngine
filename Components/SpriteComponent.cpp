#include "Components/SpriteComponent.h"
#include "GameObjects/GameObject.h"
#include "Game.h"
#include "Math.h"
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
#include "GameObjects/Camera.h"
#include "Singletons/ShaderManager.h"
#include "Renderer.h"

#pragma region コンストラクタ：デストラクタ

SpriteComponent::SpriteComponent(GameObject* parent, int drawLayer)
	: TextureComponent(parent, drawLayer){
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

		shader->SetMatrixUniform("uWorldTransform", world);
		shader->SetVector2Uniform("uTexOffset", Vector2(0.0f, 0.0f));
		shader->SetVector2Uniform("uTexScale", Vector2(1.0f, 1.0f));

		_texture->SetActive();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}

#pragma endregion

#pragma region プライベート関数


#pragma endregion

