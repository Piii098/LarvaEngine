#include "Components/Draw/DebugDrawComponent.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Camera.h"
#include "Components/Camera/CameraComponent.h"
#include "Core/Game.h"
#include "Renderer/Shader.h"
#include "AssetManagers/AssetData/Texture.h"
#include "Components/Phys/BoxComponent2D.h"

#pragma region コンストラクタ:デストラクタ

DebugDrawComponent::DebugDrawComponent(GameObject* parent, BoxComponent2D* boxComp, int drawLayer)
	: SpriteComponent(parent, drawLayer)
	, _boxComp(boxComp){
    SetTexture("RedBox");
}

DebugDrawComponent::~DebugDrawComponent() {

}

#pragma endregion

#pragma region パブリック関数

void DebugDrawComponent::SetBox(const Vector2& position, const Vector2& size, const Vector3& color) {

}

void DebugDrawComponent::Render(Shader* shader) {

    if (_boxComp && _texture) {

        // Fetch the world box from the BoxComponent2D
        const AABB2D& worldBox = _boxComp->GetWorldBox();
      
        // Calculate the scale and translation matrices
        Vector2Int boxSize = worldBox._max - worldBox._min;
        Vector2Int boxCenter = (worldBox._max + worldBox._min) / 2;
  
        Matrix4 scaleMat = Matrix4::CreateScale(boxSize.x, boxSize.y, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(boxCenter.x, boxCenter.y, 0.0f));

        Matrix4 world = scaleMat * transMat ;

        shader->SetMatrixUniform("uWorldTransform", world);
        shader->SetVector2Uniform("uTexOffset", Vector2(0.0f, 0.0f));
        shader->SetVector2Uniform("uTexScale", Vector2(1.0f, 1.0f));

        _texture->SetActive();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

}

#pragma endregion
