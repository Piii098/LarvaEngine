#include "Components/Draw/BGComponent.h"
#include "Renderer/Shader.h"
#include "GameObjects/GameObject.h"
#include "Core/Game.h"
#include "Utilities/Math.h"
#include "Renderer/Renderer.h"
#include "GameObjects/Camera.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#pragma region コンストラクタ:デストラクタ

BGComponent::BGComponent(GameObject* parent, float offsetRatio, int updateLayer)
    : TextureComponent(parent, updateLayer)
    , _offsetRatio(offsetRatio) {
    _parent->GetScene()->GetManager()->GetGame()->GetRenderer()->AddBackground(this);
}

BGComponent::~BGComponent() {
    _parent->GetScene()->GetManager()->GetGame()->GetRenderer()->RemoveBackground(this);
}

#pragma endregion

#pragma region パブリック関数
void BGComponent::Render(Shader* shader) {
    if (_texture) {
        float scaleX = _flipX ? -1.f : 1.f;
        float scaleY = _flipY ? -1.f : 1.f;

        Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(_texWidth) * scaleX, static_cast<float>(_texHeight) * scaleY, 1.0f);
        float cameraPosX = _parent->GetScene()->GetCamera()->Position().x;
        float offset = cameraPosX * (1 - _offsetRatio);

        // Calculate the translation matrix for the parallax offset
        Matrix4 translationMat = Matrix4::CreateTranslation(Vector3(-offset, 0.0f, 0.0f));

        // Calculate the world matrix based on the parent's *original* transform
        Matrix4 world = scaleMat * translationMat * _parent->WorldTransform();

        shader->SetMatrixUniform("uWorldTransform", world);

        shader->SetVector3Uniform("selfLightColor", _selfLightColor);
        shader->SetFloatUniform("selfLightIntensity", _selfLightIntensity);
        _texture->SetActive();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

#pragma endregion