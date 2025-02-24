#include "Components/BGComponent.h"
#include "Utilities/Shader.h"
#include "GameObjects/GameObject.h"
#include "Game.h"
#include "Renderer.h"

#pragma region コンストラクタ:デストラクタ

BGComponent::BGComponent(GameObject* parent)
	: TextureComponent(parent){
	_parent->GetGame()->GetRenderer()->AddBackground(this);
}

BGComponent::~BGComponent() {
	_parent->GetGame()->GetRenderer()->RemoveBackground(this);
}

#pragma endregion


#pragma region パブリック関数

void BGComponent::Draw(Shader* shader) {
	if (_texture) {
		float scaleX = _flipX ? -1.f : 1.f;
		float scaleY = _flipY ? -1.f : 1.f;

		Matrix4 scaleMat = Matrix4::CreateScale(scaleX * static_cast<float>(_texWidth), scaleY * static_cast<float>(_texHeight), 1.0f);
		Matrix4 world = scaleMat * _parent->WorldTransform();

		_texture->SetActive();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

#pragma endregion
