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

	SpriteComponent& spriteComp = CreateComponent<SpriteComponent>(10);
	spriteComp.SetTexture("Player");

	RigidbodyComponent& rigidbodyComp = CreateComponent<RigidbodyComponent>(true);
	rigidbodyComp.Mass(5);
	rigidbodyComp.IsGravity(true);

	MoveInputComponent& moveInputComp = CreateComponent<MoveInputComponent>();
	moveInputComp.MoveSpeedX(80.f);
	moveInputComp.State(Component::STATE::INACTIVE);


}

Example2DScroll::Player::~Player() {

}

/// ====================================================================================================
/// パブリック関数
///	====================================================================================================

void Example2DScroll::Player::UpdateObject(float deltaTime) {
	//SDL_Log("position : %f, %f", _rigidbodyComp->InternalPosition().x, _rigidbodyComp->InternalPosition().y);
	//SDL_Log("position : %d, %d", Position().x, Position().y);
}