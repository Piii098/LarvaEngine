#pragma once
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/Utilities/Math.h"

class RigidbodyComponent;

class MoveInputComponent: public Component {
public:

	MoveInputComponent(GameObject& parent, RigidbodyComponent* rigidbodyComp, int updateLayer = 100);
	~MoveInputComponent() override;

	void ProcessInput(const InputAction& input) override;
	void FixedUpdate(float deltaTime) override;


	/*ゲッターセッター*/

	void MoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; };
	void MoveSpeedX(float moveSpeed) { _moveSpeedX = moveSpeed; };
	void MoveSpeedY(float moveSpeed) { _moveSpeedY = moveSpeed; };
	void JumpForce(float jumpForce) { _jumpForce = jumpForce; };

	Vector2& Direction() { return _direction; };

	void MaxHorizontalForce(float maxHorizontalForce) { _maxHorizontalForce = maxHorizontalForce; };
	void MaxVerticalForce(float maxVerticalForce) { _maxVerticalForce = maxVerticalForce; };
	void MaxHorizontalVelocity(float maxHorizontalVelocity) { _maxHorizontalVelocity = maxHorizontalVelocity; };
	void MaxVerticalVelocity(float maxVerticalVelocity) { _maxVerticalVelocity = maxVerticalVelocity; };
private:

	RigidbodyComponent* _rigidbodyComp;

	float _maxVerticalForce;
	float _maxHorizontalForce;
	float _maxVerticalVelocity;
	float _maxHorizontalVelocity;

	float _moveSpeed;
	float _moveSpeedX;
	float _moveSpeedY;
	float _jumpForce;
	bool _isJumping;

	Vector2 _direction;
};