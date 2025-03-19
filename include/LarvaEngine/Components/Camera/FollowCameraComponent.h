#pragma once
#include "LarvaEngine/Components/Camera/CameraComponent.h"

class FollowCameraComponent : public CameraComponent {
public:
    FollowCameraComponent(GameObject* parent, int updateLayer = 1000);
    ~FollowCameraComponent() override;

    void Update(float deltaTime) override;
    void PhysUpdate(float deltaTime) override;

    // オフセット関連
    void XOffset(float offset) { _xOffset = offset; }
    void YOffset(float offset) { _yOffset = offset; }
    float GetXOffset() const { return _xOffset; }
    float GetYOffset() const { return _yOffset; }

    // 減衰率
    void AttenRate(float rate) { _attenRate = rate; }
    float GetAttenRate() const { return _attenRate; }

    // 内部位置
    Vector2 GetPositionFloat() const { return _internalPosition; }

private:
    
    float _xOffset;       // X軸オフセット
    float _yOffset;       // Y軸オフセット
    float _attenRate;     // 追従の減衰率
    Vector2 _internalPosition;  // 内部的な浮動小数点位置
    Vector2 _prevInternalPosition;
    Vector2 _position;
	Vector2 _targetPos;
    Vector2 _currentVelocity;
};