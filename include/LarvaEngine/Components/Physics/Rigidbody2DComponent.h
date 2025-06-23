#pragma once
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/Utilities/Math.h"

class DynamicObject;
class PhysWorld2D;

class Rigidbody2DComponent : public Component {
public:

    Rigidbody2DComponent(GameObject& parent, int updateLayer = 30);
    ~Rigidbody2DComponent() override;

    void Update(float deltaTime) override;  // 新しいUpdate関数を追加
    void FixedUpdate(float deltaTime) override;
    void CalculateVelocity(float deltaTime);
    void UpdatePosition(float deltaTime);

    void AddForce(Vector2 force);

    float AngularSpeed() const { return _angularSpeed; };
    float ForwardSpeed() const { return _forwardSpeed; };
    void AngularSpeed(float angularSpeed) { _angularSpeed = angularSpeed; };
    void ForwardSpeed(float forwardSpeed) { _forwardSpeed = forwardSpeed; };

    void Mass(float mass) { _mass = mass; };
    void Velocity(Vector2 velocity) { _velocity = velocity; };
    void IsGravity(bool frag) { _isGravity = frag; };
    bool IsGravity() const { return _isGravity; };
    bool OnGround() const { return _onGround; }; // 地面に接触しているかどうか
    float Mass() { return _mass; };
    const Vector2& Velocity() const { return _velocity; };
    void Drag(float drag) { _drag = drag; };
    Vector2 InternalPosition() const { return _internalPosition; };
    void InternalPosition(Vector2 internalPosition) { _internalPosition = internalPosition; };

private:

    float _angularSpeed;
    float _forwardSpeed;

    float _mass;
    float _drag;
    Vector2 _sumOfForces;
    Vector2 _velocity;

    Vector2 _nextPosition;
    Vector2 _internalPosition;

    bool _isGravity;
    bool _onGround; // 地面に接触しているかどうか
};