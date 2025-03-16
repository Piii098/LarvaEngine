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
#include "Scene/MainScene.h"
#include "GameObjects/TestObject.h"
#include "Scene/Scene.h"
#include "Components/Draw/TextComponent.h"
#include "Components/Audio/AudioComponent.h"
#include "Audio/SoundEvent.h"

#pragma region コンストラクタ:デストラクタ

Player::Player(Scene* scene)
	: GameObject(scene) {
	Tag(TAG::PLAYER);
	Scale(1.f);
	Position(Vector2Int(50, 500));
	_spriteComp = new SpriteComponent(this, 10, 100);
	_spriteComp->SetTexture("Player");
	_spriteComp->SelfLightIntensity(0.1f);

	_jumpForce = 700.f;

	_rigidbodyComp = new RigidbodyComponent(this, true);
	_boxComp = new BoxComponent2D(this, true, true);
	AABB2D myBox(Vector2Int(-6.0f, -8.f), Vector2Int(6.0f, 6.f));
	_boxComp->SetObjectBox(myBox);
	_rigidbodyComp->Velocity(Vector2::Zero);
	_rigidbodyComp->Mass(50.f);
	_rigidbodyComp->IsGravity(true);
	_rigidbodyComp->Drag(0.f);
	_rigidbodyComp->SetInterpolationMode(RigidbodyComponent::InterpolationMode::Interpolate);

	_moveInputComp = new MoveInputComponent(this, _rigidbodyComp);
	_moveInputComp->MoveSpeedX(80.f);
	_moveInputComp->JumpForce(1000000.f);
	_moveInputComp->SetState(Component::STATE::INACTIVE);
	
	GetMainScene()->SetData("Player.Position.X", Position().x);
	GetMainScene()->SetGetter("Player.Position.X", [this]() {return Position().x; });

	_audioComp = new AudioComponent(this);
	_footstepEvent = _audioComp->PlayEvent("event:/Footsteps_grass");
	_footstepEvent.SetVolume(0.1);
	_footstepEvent.Stop();
	//TextComponent* textComp = new TextComponent(this, 15, "DelaSuko");
	//textComp->CreateTextTexture("PLAYER", Vector3::fromIntRGB(255, 0, 0), 30);

	//new DebugDrawComponent(this, _boxComp, true);
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


}

void Player::UpdateObject(float deltaTime) {
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


#pragma endregion

