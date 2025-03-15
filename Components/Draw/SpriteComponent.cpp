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

SpriteComponent::SpriteComponent(GameObject* parent, int bufferLayer, int drawLayer)
	: TextureComponent(parent, bufferLayer){
	//_parent->GetScene()->GetManager()->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	//_parent->GetScene()->GetManager()->GetGame()->GetRenderer()->RemoveSprite(this);
}

#pragma endregion

#pragma region パブリック関数

#pragma endregion

#pragma region プライベート関数


#pragma endregion

