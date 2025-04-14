#pragma once
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/Utilities/Math.h"

class DynamicObject;
class PhysWorld2D;

class RigidbodyComponent : public Component {
public:
    // 補間モードの列挙型を追加
    enum class InterpolationMode {
        None,       // 補間なし
        Interpolate // 前回と現在の物理位置を補間
    };

    RigidbodyComponent(GameObject& parent, int updateLayer = 30);
    ~RigidbodyComponent() override;

    void Update(float deltaTime) override;  // 新しいUpdate関数を追加
    void PhysUpdate(float deltaTime) override;
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
    void Drag(float drag) { _drag = drag; };
    float Mass() { return _mass; };
    const Vector2& Velocity() const { return _velocity; };

    Vector2 InternalPosition() { return _internalPosition; };
    void InternalPosition(Vector2 internalPosition); // 実装を変更

    // 補間モードの設定
    void SetInterpolationMode(InterpolationMode mode) { _interpolationMode = mode; }
    InterpolationMode GetInterpolationMode() const { return _interpolationMode; }

private:
    PhysWorld2D& _phys;
    float _angularSpeed; 
    float _forwardSpeed; 

    float _mass; 
    Vector2 _sumOfForces;
    Vector2 _velocity;

    Vector2 _nextPosition;
    Vector2 _internalPosition;
    Vector2 _prevInternalPosition;  // 前回の物理位置

    float _drag;
    InterpolationMode _interpolationMode;  // 補間モード

    bool _isGravity;
};