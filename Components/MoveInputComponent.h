#pragma once
#include "Components/Component.h"

class Input;
class RigidbodyComponent;

class MoveInputComponent: public Component {
public:

	MoveInputComponent(GameObject* parent, RigidbodyComponent* rigidbodyComp, int updateLayer = 100);
	~MoveInputComponent() override;

	void ProcessInput(Input* input) override;
	void Update(Frame* frame) override;


	/*ゲッターセッター*/

	void MoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; };
	void JumpForce(float jumpForce) { _jumpForce = jumpForce; };

	int Direction() { return _direction; };

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
	float _jumpForce;

	int _direction;
};