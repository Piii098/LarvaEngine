#include "LarvaEngine/Components/Physics/MoveInputComponent.h"
#include "LarvaEngine/Components/Physics/RigidbodyComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Input/InputAction.h"

MoveInputComponent::MoveInputComponent(GameObject& parent, RigidbodyComponent* rigidbodyComp, int updateLayer)
	: Component(parent, updateLayer)
	, _rigidbodyComp(rigidbodyComp)
	, _moveSpeed(0.f)
	, _moveSpeedX(0.f)
	, _moveSpeedY(0.f)
	, _jumpForce(0.f)
	, _isJumping(false)
	, _maxVerticalForce(0.f)
	, _maxHorizontalForce(0.f)
	, _maxVerticalVelocity(0.f)
	, _maxHorizontalVelocity(0.f){
	
}

MoveInputComponent::~MoveInputComponent() {

}


#pragma region パブリック関数

void MoveInputComponent::ProcessInput(const InputAction& input) {
	_direction = Vector2::Zero;

	if (input.IsKey(SDL_SCANCODE_RIGHT)) {
		_direction.x += 1;
	}
	if (input.IsKey(SDL_SCANCODE_LEFT)) {
		_direction.x -= 1;
	}
	if (input.IsKey(SDL_SCANCODE_UP)) {
		_direction.y += 1;
	}
	if (input.IsKey(SDL_SCANCODE_DOWN)) {
		_direction.y -= 1;
	}

	PhysWorld2D& phys = _parent.GetScene().GetManager().GetGame().GetPhysWorld();
	PhysWorld2D::CollisionInfo outColl;

	_parent.ComputeWorldTransform();

	// プレイヤーの少し下から地面方向へレイを飛ばす
	Vector2Int rayStart = _parent.Position() + Vector2Int(0.f, -1.0f);  // プレイヤーの足元あたり
	LineSegment2D ray(rayStart, rayStart + Vector2Int(0.f, -15));
	if (phys.SegmentCast(ray, outColl, _parent)) {
		if (input.IsKeyDown(SDL_SCANCODE_UP) && outColl._object->Tag() == GameObject::TAG::GROUND) {
			_isJumping = true;
		}
	}
}

void MoveInputComponent::PhysUpdate(float deltaTime){
	Vector2 currentVelocity = _rigidbodyComp->Velocity();

	Vector2 desiredVelocity(static_cast<float>(_direction.x) * (_moveSpeed + _moveSpeedX), currentVelocity.y);
	Vector2 velocityChange = desiredVelocity - currentVelocity;

	// 質量を考慮して力を計算
	float mass = _rigidbodyComp->Mass(); // 新たに質量を取得する必要があります
	Vector2 force = velocityChange * mass / deltaTime;

	if (_isJumping) {
		force.y += _jumpForce;
		_isJumping = false;
	}

	_rigidbodyComp->AddForce(force);
	_direction = Vector2::Zero;
}

#pragma endregion
