#include "LarvaEngine/Components/Physics/MoveInputComponent.h"
#include "LarvaEngine/Components/Physics/RigidbodyComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Input/InputAction.h"

MoveInputComponent::MoveInputComponent(GameObject& parent, int updateLayer)
	: Component(parent, updateLayer)
	, _moveSpeed(0.f)
	, _moveSpeedX(0.f)
	, _moveSpeedY(0.f)
	, _jumpForce(0.f)
	, _isJumping(false){
	//SDL_Log("MoveInputComponent::MoveInputComponent\n");
	_rigidbodyComp = _parent.GetComponent<RigidbodyComponent>();
	
}

MoveInputComponent::~MoveInputComponent() {

}


#pragma region パブリック関数

void MoveInputComponent::ProcessInput(const InputAction& input) {
	_direction = 0;
	_inputActive = false;

	float horizontal = input.GetActionValue("Horizontal");

	if (horizontal != 0.f) {
		_direction = horizontal;
		_inputActive = true;
	}

	if (input.IsActionDown("Jump") > 0.f) {
		_isJump = true;
	}

}

void MoveInputComponent::Update(float deltaTime) {

}


void MoveInputComponent::FixedUpdate(float deltaTime){

	float newHori = _direction * 10000 * deltaTime;

	if (_isJump && _rigidbodyComp->OnGround() ) {
		// ジャンプ処理
		SDL_Log("Jump");
		_rigidbodyComp->AddForce(Vector2(0, 100000));
		_isJump = false;

	}

	Vector2 newVelo = Vector2(newHori, _rigidbodyComp->Velocity().y);

	_rigidbodyComp->Velocity(newVelo);

	/*
	SDL_Log("dir: %f, %f", _direction.x, _direction.y);
	Vector2 newPos = _parent.PositionToFloat();
	SDL_Log("newPos: %f, %f", newPos.x, newPos.y);
	newPos += _direction * 100 * deltaTime;
	SDL_Log("newPos: %f, %f", newPos.x, newPos.y);
	_parent.Position(newPos);
	*/
}

#pragma endregion
