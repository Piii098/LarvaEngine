#include "LarvaEngine/Examples/Game/2DScroll/Player.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Components/Physics/RigidbodyComponent.h"
#include "LarvaEngine/Components/Physics/MoveInputComponent.h"
#include "LarvaEngine/Components/Physics/BoxComponent2D.h"
#include <SDL3/SDL.h>

/// ====================================================================================================
/// コンストラクタ・デストラクタ
/// ====================================================================================================

Example2DScroll::Player::Player(Scene& scene)
	: GameObject(scene){

	_spriteComp = &CreateComponent<SpriteComponent>(10);
	_spriteComp->SetTexture("Player");

	_rigidbodyComp = &CreateComponent<RigidbodyComponent>(true);
	_rigidbodyComp->Mass(1.0f);
	_rigidbodyComp->Drag(0.f);
	_rigidbodyComp->IsGravity(true);

	_moveInputComp = &CreateComponent<MoveInputComponent>(_rigidbodyComp);
	_moveInputComp->MoveSpeedX(80.f);
	_moveInputComp->State(Component::STATE::INACTIVE);


}

Example2DScroll::Player::~Player() {

}

/// ====================================================================================================
/// パブリック関数
///	====================================================================================================

void Example2DScroll::Player::UpdateObject(float deltaTime) {
	
}