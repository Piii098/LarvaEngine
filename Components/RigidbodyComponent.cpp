#include "Components/RigidbodyComponent.h"
#include "GameObjects/GameObject.h"
#include "Math.h"
#include "Utilities/Frame.h"
#include "PhysWorld2D.h"
#include "Game.h"

RigidbodyComponent::RigidbodyComponent(GameObject* parent, int updateLayer)
	: Component(parent, updateLayer)
	, _angularSpeed(0.f)
	, _forwardSpeed(0.f)
	, _mass(0.f)
	, _sumOfForces(Vector2::Zero)
	, _velocity(Vector2::Zero)
	, _isGravity(false){

}

RigidbodyComponent::~RigidbodyComponent() {

}

#pragma region パブリック関数

void RigidbodyComponent::Update(Frame* frame) {

	if (_isGravity) {
		float gravityForce = _mass * 300.f ;
		AddForce(Vector2(0, -gravityForce));
	}
	
	/*ニュートン力学*/
	Vector2 accele; // 加速度を割り出す
	accele.x = _sumOfForces.x / _mass; // f = ma -> a = f / m
	accele.y = _sumOfForces.y / _mass;

	_velocity += accele * frame->DeltaTime();

	_velocity *= (1.0f - _drag * frame->DeltaTime());

	Vector2 pos = _parent->Position();
	
	pos += _velocity * frame->DeltaTime();

	if (pos.y < 20.f) {
		pos.y = 20.f;
		_velocity.y = 0;
	}

	_parent->Position(pos);

	_sumOfForces = Vector2::Zero;

}

void RigidbodyComponent::AddForce(Vector2 force) {
	_sumOfForces += force;
}


#pragma endregion
