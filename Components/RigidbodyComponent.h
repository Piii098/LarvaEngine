#pragma once
#include "Components/Component.h"
#include "Math.h"

class DynamicObject;

class RigidbodyComponent: public Component {
public:

	RigidbodyComponent(GameObject* parent, int updateLayer = 30);
	~RigidbodyComponent() override;

	void Update(Frame* frame) override; //ニュートン力学を計算

	void AddForce(Vector2 force);

	/*ゲッターセッター*/

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

	float _angularSpeed; //回転速度
	float _forwardSpeed; //前方運動速度

	/*ニュートン力学*/
	float _mass; //質量
	Vector2 _sumOfForces; //力の合計
	Vector2 _velocity; //速度
	
	float _drag;

	bool _isGravity;
};