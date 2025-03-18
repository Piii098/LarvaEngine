#include "Components/Draw/LightComponent.h"
#include <gl/glew.h>
#include "GameObjects/GameObject.h"
#include "Renderer/Shader.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Core/Game.h"
#include "Renderer/Renderer.h"

#pragma region コンストラクタ:デストラクタ

LightComponent::LightComponent(GameObject* parent, int bufferLayer, int updateLayer)
	: Component(parent, updateLayer)
    , _lightColor(Vector3(1.f, 1.f, 1.f))
    , _lightIntensity(1.f)
    , _lightScale(Vector2(1.f, 1.f))
    , _lightRange(50)
    , _lightOffset(Vector2::Zero){
	SetBufferLayer(bufferLayer);
	GetParent()->GetScene()->GetManager()->GetGame()->GetRenderer()->AddLight(this);
}

LightComponent::~LightComponent() {
    GetParent()->GetScene()->GetManager()->GetGame()->GetRenderer()->RemoveLight(this);
}


#pragma endregion

#pragma region パブリック関数

void LightComponent::RenderLight(Shader* shader) {

    // スケールマトリックスの作成
    Matrix4 scaleMat = Matrix4::CreateScale(1000, 1000,1.0f);

    // 平行移動マトリックスの作成（水平方向の揃えを考慮）
    Matrix4 translationMat = Matrix4::CreateTranslation(Vector3(0, 0, 0));

    // ワールド変換マトリックスの計算
    Matrix4 world = scaleMat * translationMat * _parent->WorldTransform();

    // 2D化したライト中心をシェーダへ送る（計算体系に合わせて調整）
    shader->SetVector2Uniform("uLightCenter", _parent->Position() + Vector2Int::ToInterger(_lightOffset));
	shader->SetMatrixUniform("uWorldTransform", world);
    shader->SetVector3Uniform("uLightColor", _lightColor);

    shader->SetFloatUniform("uLightIntensity", _lightIntensity);  // 強度を設定
    shader->SetFloatUniform("uLightRange", _lightRange);          // 範囲を設定


    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

#pragma endregion

