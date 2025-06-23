#include "LarvaEngine/Components/Draw/DebugDrawComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Components/Camera/CameraComponent.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Components/Physics/Box2DComponent.h"

#pragma region コンストラクタ:デストラクタ

DebugDrawComponent::DebugDrawComponent(GameObject& parent, Box2DComponent* boxComp)
	: SpriteComponent(parent, 16)
	, _boxComp(boxComp){
    SetTexture("RedBox");
}

DebugDrawComponent::~DebugDrawComponent() {

}

#pragma endregion

#pragma region パブリック関数

void DebugDrawComponent::SetBox(const Vector2& position, const Vector2& size, const Vector3& color) {

}

#pragma endregion
