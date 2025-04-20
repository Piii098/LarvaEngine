#include "LarvaEngine/GameObjects/Player.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Components/Physics/MoveInputComponent.h"
#include "LarvaEngine/Components/Physics/RigidbodyComponent.h"
#include "LarvaEngine/Core/Game.h"
#include <SDL3/SDL.h>
#include "LarvaEngine/Components/TileMapComponent.h"
#include "LarvaEngine/Physics/Collision2D.h"
#include "LarvaEngine/Components/Physics/BoxComponent2D.h"
#include "LarvaEngine/Components/Draw/DebugDrawComponent.h"
#include <iostream>
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/Core/MainScene.h"

#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"
#include "LarvaEngine/Components/Audio/AudioComponent.h"
#include "LarvaEngine/Audio/SoundEvent.h"
#include "LarvaEngine/Components/Light/LightComponent.h"

#pragma region コンストラクタ:デストラクタ

Player::Player(Scene& scene)
	: GameObject(scene) {
	Tag(TAG::PLAYER);
	Scale(1.f);
	Position(Vector2Int(50, 500));
	_spriteComp = CreateComponent<SpriteComponent>(10, 100);
	_spriteComp->SetTexture("Player");
	_spriteComp->SelfLightIntensity(0.1f);

	_jumpForce = 700.f;

	_rigidbodyComp = CreateComponent<RigidbodyComponent>();
	_boxComp = CreateComponent<BoxComponent2D>(true, true);
	AABB2D myBox(Vector2Int(-6.0f, -8.f), Vector2Int(6.0f, 6.f));
	_boxComp->SetObjectBox(myBox);
	_rigidbodyComp->Velocity(Vector2::Zero);
	_rigidbodyComp->Mass(50.f);
	_rigidbodyComp->IsGravity(true);

	_moveInputComp = CreateComponent<MoveInputComponent>(_rigidbodyComp);
	_moveInputComp->MoveSpeedX(80.f);
	_moveInputComp->JumpForce(1000000.f);
	_moveInputComp->State(Component::STATE::INACTIVE);
	
	GetMainScene().SetData("Player.Position.X", Position().x);
	GetMainScene().SetDataUpdate("Player.Position.X", [this]() {return Position().x; });

	_audioComp = CreateComponent<AudioComponent>();
	_footstepEvent = _audioComp->PlayEvent("event:/Footsteps_grass");
	_footstepEvent.SetVolume(0.1);
	_footstepEvent.Stop();

	_lightComp = CreateComponent<LightComponent>(10, 100);
	_lightComp->LightRange(10.f);
	_lightComp->LightColor(Vector3(1,0.2,0.2));
	_lightComp->LightIntensity(0.3f);

	//TextComponent* textComp = new TextComponent(this, 15, "DelaSuko");
	//textComp->CreateTextTexture("PLAYER", Vector3::fromIntRGB(255, 0, 0), 30);

	SDL_Log("Player was created");
	//new DebugDrawComponent(this, _boxComp, true);
}

Player::~Player() {

}
#pragma endregion


#pragma region パブリック関数

void Player::InputObject(const InputAction& input) {


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

