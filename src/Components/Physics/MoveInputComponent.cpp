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
	_direction = Vector2::Zero;
	_inputActive = false;

	float horizontal = input.GetActionValue("Horizontal");
	float vertical = -input.GetActionValue("Vertical");

	if (horizontal != 0.f) {
		_direction.x = horizontal;
		_inputActive = true;
	}
	if (vertical != 0.f) {
		_direction.y = vertical;
		_inputActive = true;
	}

	// 入力がある場合のみ正規化する
	if(_inputActive) {
		_direction.Normalize();
	}


}

void MoveInputComponent::Update(float deltaTime) {

}


void MoveInputComponent::FixedUpdate(float deltaTime){

	Vector2 newVelo = _direction * 10000 * deltaTime;
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
