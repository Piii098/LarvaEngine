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
	, _isJumping(false)
	, _maxVerticalForce(0.f)
	, _maxHorizontalForce(0.f)
	, _maxVerticalVelocity(0.f)
	, _maxHorizontalVelocity(0.f){
	//SDL_Log("MoveInputComponent::MoveInputComponent\n");
	
}

MoveInputComponent::~MoveInputComponent() {

}


#pragma region パブリック関数

void MoveInputComponent::ProcessInput(const InputAction& input) {
	_direction = Vector2::Zero;

	if (input.IsKey(SDL_SCANCODE_RIGHT)) {
		//SDL_Log("RIGHT KEY PRESSED\n");
		_direction.x += 1;
	}
	if (input.IsKey(SDL_SCANCODE_LEFT)) {
		//SDL_Log("LEFT KEY PRESSED\n");
		_direction.x -= 1;
	}
	if (input.IsKey(SDL_SCANCODE_UP)) {
		_direction.y += 1;
	}
	if (input.IsKey(SDL_SCANCODE_DOWN)) {
		_direction.y -= 1;
	}

	if (input.IsKeyDown(SDL_SCANCODE_SPACE) && _isJumping == false) {
		_isJumping = true;
	} 
}

void MoveInputComponent::FixedUpdate(float deltaTime){
	Vector2 force = Vector2::Zero;

	force += _direction * 100000 * deltaTime;

	if (_isJumping) {
		force.y += 100000;
		_isJumping = false;
	}

	//SDL_Log("Force(%f, %f)\n", force.x, force.y);
	RigidbodyComponent* rigidbodyComp = _parent.GetComponent<RigidbodyComponent>();
	rigidbodyComp->AddForce(force);
	_direction = Vector2::Zero;
}

#pragma endregion
