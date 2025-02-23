#include "Components/MoveInputComponent.h"
#include "Components/RigidbodyComponent.h"
#include "Math.h"
#include "GameObjects/GameObject.h"
#include "Utilities/Frame.h"
#include "Utilities/Input.h"
#include "SDL3/SDL.h"
#include "Components/Component.h"
#include <iostream>
#include "PhysWorld2D.h"
#include "Game.h"

MoveInputComponent::MoveInputComponent(GameObject* parent, RigidbodyComponent* rigidbodyComp)
	: Component(parent)
	, _rigidbodyComp(rigidbodyComp)
	, _moveSpeed(0.f)
	, _jumpForce(0.f)
	, _maxVerticalForce(0.f)
	, _maxHorizontalForce(0.f)
	, _maxVerticalVelocity(0.f)
	, _maxHorizontalVelocity(0.f){
	
}

MoveInputComponent::~MoveInputComponent() {

}


#pragma region パブリック関数

void MoveInputComponent::ProcessInput(Input* input) {

	_direction = 0;

	if (input->IsInput(InputMap::INPUT_DRIGHT)) {
		_direction += 1;
	}
	if (input->IsInput(InputMap::INPUT_DLEFT)) {
		_direction -= 1;
	}
	

	PhysWorld2D* phys = _parent->GetGame()->GetPhysWorld();
	PhysWorld2D::CollisionInfo outColl;

	_parent->ComputeWorldTransform();

	// プレイヤーの少し下から地面方向へレイを飛ばす
	Vector2 rayStart = _parent->Position() + Vector2(0.f, -1.0f);  // プレイヤーの足元あたり
	LineSegment2D ray(rayStart, rayStart + Vector2(0.f, -15));
	if (phys->SegmentCast(ray, outColl, _parent)) {
		if (input->IsInputDown(InputMap::INPUT_DUP) && outColl._object->Tag() == GameObject::TAG::GROUND) {
			_rigidbodyComp->AddForce(Vector2(0.f, _jumpForce));
		}
	}


	/*
	float horizontalForce = 0.f;
	float verticalForce = 0.f;

	if (input->IsInput(InputMap::INPUT_DRIGHT)) {
		horizontalForce += _maxHorizontalForce;
	}
	if (input->IsInput(InputMap::INPUT_DLEFT)) {
		horizontalForce -= _maxHorizontalForce;
	}
	if (input->IsInput(InputMap::INPUT_DUP)) {
		verticalForce += _maxVerticalForce;
	}
	if (input->IsInput(InputMap::INPUT_DDOWN)) {
		verticalForce -= _maxVerticalForce;
	}
	
	_rigidbodyComp->AddForce(Vector2(horizontalForce, verticalForce));
	*/
	
	/*
	float horizontalVeclocity = 0.f;
	float verticalVeclocity = 0.f;

	if (input->IsInput(InputMap::INPUT_DRIGHT)) {
		horizontalVeclocity += _maxHorizontalVelocity;
	}
	if (input->IsInput(InputMap::INPUT_DLEFT)) {
		horizontalVeclocity -= _maxHorizontalVelocity;
	}
	if (input->IsInput(InputMap::INPUT_DUP)) {
		verticalVeclocity += _maxVerticalVelocity;
	}
	if (input->IsInput(InputMap::INPUT_DDOWN)) {
		verticalVeclocity -= _maxVerticalVelocity;
	}
	_rigidbodyComp->Velocity(Vector2(horizontalVeclocity, verticalVeclocity));
	*/

}

void MoveInputComponent::Update(Frame* frame){
	_rigidbodyComp->Velocity(Vector2(static_cast<float>(_direction) * _moveSpeed, _rigidbodyComp->Velocity().y));
}

#pragma endregion
