#pragma once
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/Utilities/Math.h"

class Rigidbody2DComponent;

class MoveInputComponent: public Component {
public:

	MoveInputComponent(GameObject& parent, int updateLayer = 100);
	~MoveInputComponent() override;

	void ProcessInput(const InputAction& input) override;
	void Update(float deltaTime) override;
	void FixedUpdate(float deltaTime) override;


	/*ゲッターセッター*/

	void MoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; };
	void MoveSpeedX(float moveSpeed) { _moveSpeedX = moveSpeed; };
	void MoveSpeedY(float moveSpeed) { _moveSpeedY = moveSpeed; };
	void JumpForce(float jumpForce) { _jumpForce = jumpForce; };

	float& Direction() { return _direction; };

private:

	float _moveSpeed;
	float _moveSpeedX;
	float _moveSpeedY;
	float _jumpForce;
	bool _isJumping;
	bool _inputActive;

	Rigidbody2DComponent* _rigidbodyComp;
	bool _isJump;
	float _direction;
};