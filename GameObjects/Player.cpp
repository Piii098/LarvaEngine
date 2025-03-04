#include "GameObjects/Player.h"
#include "Components/Draw/SpriteComponent.h"
#include "Components/MoveInputComponent.h"
#include "Components/Phys/RigidbodyComponent.h"
#include "Core/Game.h"
#include "Utilities/Input.h"
#include <SDL3/SDL.h>
#include "Components/TileMapComponent.h"
#include "Utilities/Collision2D.h"
#include "Components/Phys/BoxComponent2D.h"
#include "Components/Draw/DebugDrawComponent.h"
#include <iostream>
#include "Utilities/Frame.h"
#include "Phys/PhysWorld2D.h"
#include "GameObjects/TestObject.h"

#pragma region コンストラクタ:デストラクタ

Player::Player(Game* game) 
	: GameObject(game) {
	Tag(TAG::PLAYER);
	Scale(1.f);
	Position(Vector2Int(50, 500));
	_spriteComp = new SpriteComponent(this, 100);
	_spriteComp->SetTexture("Player");
	_spriteComp->SelfLightIntensity(0.1f);

	_jumpForce = 700.f;

	_rigidbodyComp = new RigidbodyComponent(this, true);
	_boxComp = new BoxComponent2D(this, true, true);
	AABB2D myBox(Vector2Int(-6.0f, -16.f), Vector2Int(6.0f, 13.0f));
	_boxComp->SetObjectBox(myBox);
	_rigidbodyComp->Velocity(Vector2::Zero);
	_rigidbodyComp->Mass(50.f);
	_rigidbodyComp->IsGravity(true);
	_rigidbodyComp->Drag(0.f);
	_rigidbodyComp->SetInterpolationMode(RigidbodyComponent::InterpolationMode::Interpolate);

	_moveInputComp = new MoveInputComponent(this, _rigidbodyComp);
	_moveInputComp->MoveSpeedX(80.f);
	_moveInputComp->JumpForce(1000000.f);

	new DebugDrawComponent(this, _boxComp, true);
}

Player::~Player() {
	delete _spriteComp;
	_spriteComp = nullptr;
	delete _moveInputComp;
	_moveInputComp = nullptr;
	delete _rigidbodyComp;
	_rigidbodyComp = nullptr;

}
#pragma endregion


#pragma region パブリック関数

void Player::InputObject(Input* input) {
	if (input->IsInputDown(InputMap::INPUT_BRIGHT)) {
		//TestObject* bullet = new TestObject(GetGame());
		//bullet->Position(Position());
	}
}

void Player::UpdateObject(Frame* frame) {
	//SDL_Log("PlayerVelocity(%f, %f)", _rigidbodyComp->Velocity().x, _rigidbodyComp->Velocity().y);
	// 右向きに移動したら
	if (_moveInputComp->Direction().x < 0.f) {
		_spriteComp->FlipX(false);
	}
	// 左向きに移動したら
	else if (_moveInputComp->Direction().x > 0.f) {
		_spriteComp->FlipX(true);
	}

	// その他の更新処理
	// SDL_Log("WorldBoxMin(%d, %d), WorldBoxMax(%d, %d)", _boxComp->WorldMin().x, _boxComp->WorldMin().y, _boxComp->WorldMax().x, _boxComp->WorldMax().y);
	// SDL_Log("ObjectBoxMin(%d, %d), ObjectBoxMax(%d, %d)", _boxComp->ObjectMin().x, _boxComp->ObjectMin().y, _boxComp->ObjectMax().x, _boxComp->ObjectMax().y);
}

void Player::FixCollision(Frame frame) {
	
}

#pragma endregion

