#pragma once
#include <vector>
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/Utilities/Math.h"

class DynamicObject;
class PhysWorld2D;

struct Rotation {
	Vector3 axis;
	float speed;
	Rotation(Vector3 axis, float speed) : axis(axis), speed(speed) {}
};

class Rigidbody3DComponent : public Component {
public:

    Rigidbody3DComponent(GameObject& parent, int updateLayer = 30);
    ~Rigidbody3DComponent() override;

    void Update(float deltaTime) override;  // êVÇµÇ¢Updateä÷êîÇí«â¡
    void FixedUpdate(float deltaTime) override;
    void CalculateVelocity(float deltaTime);
    void UpdatePosition(float deltaTime);

    void Move(float forward, float strafe);
    void MoveRotate(const Vector3& axis, float angularSpeed);

    void AddForce(Vector3 force);

    float AngularSpeed() const { return _angularSpeed; };
    float ForwardSpeed() const { return _forwardSpeed; };
    void AngularSpeed(float angularSpeed) { _angularSpeed = angularSpeed; };
    void ForwardSpeed(float forwardSpeed) { _forwardSpeed = forwardSpeed; };

    void Mass(float mass) { _mass = mass; };
    void Velocity(Vector3 velocity) { _velocity = velocity; };
    void IsGravity(bool frag) { _isGravity = frag; };
    bool IsGravity() const { return _isGravity; };
    bool OnGround() const { return _onGround; }; // ínñ Ç…ê⁄êGÇµÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
    float Mass() { return _mass; };
    const Vector3& Velocity() const { return _velocity; };
    void Drag(float drag) { _drag = drag; };
    Vector3 InternalPosition() const { return _internalPosition; };
    void InternalPosition(Vector3 internalPosition) { _internalPosition = internalPosition; };

private:

    float _angularSpeed;
    float _forwardSpeed;
	std::vector<Rotation> _rotations;

    float _mass;
    float _drag;
    Vector3 _sumOfForces;
    Vector3 _velocity;
	
    Vector3 _nextPosition;
    Vector3 _internalPosition;

    bool _isGravity;
    bool _onGround; // ínñ Ç…ê⁄êGÇµÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
};