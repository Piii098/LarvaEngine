#pragma once
#include "Components/Component.h"
#include "Math.h"

class DynamicObject;

class RigidbodyComponent: public Component {
public:

	RigidbodyComponent(GameObject* parent, int updateLayer = 30);
	~RigidbodyComponent() override;

	void Update(Frame* frame) override; //�j���[�g���͊w���v�Z

	void AddForce(Vector2 force);

	/*�Q�b�^�[�Z�b�^�[*/

	float AngularSpeed() const { return _angularSpeed; };
	float ForwardSpeed() const { return _forwardSpeed; };
	void AngularSpeed(float angularSpeed) { _angularSpeed = angularSpeed; };
	void ForwardSpeed(float forwardSpeed) { _forwardSpeed = forwardSpeed; };

	void Mass(float mass) { _mass = mass; };
	void Velocity(Vector2 velocity) { _velocity = velocity; };
	void IsGravity(bool frag) { _isGravity = frag; };
	void Drag(float drag) { _drag = drag; };
	float Mass() { return _mass; };
	const Vector2& Velocity() const { return _velocity; };


private:

	float _angularSpeed; //��]���x
	float _forwardSpeed; //�O���^�����x

	/*�j���[�g���͊w*/
	float _mass; //����
	Vector2 _sumOfForces; //�͂̍��v
	Vector2 _velocity; //���x
	
	float _drag;

	bool _isGravity;
};