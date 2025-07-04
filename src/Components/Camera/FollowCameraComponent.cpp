﻿#include "LarvaEngine/Components/Camera/FollowCameraComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include <SDL3/SDL.h>

#pragma region シングルトン関連

#pragma endregion


#pragma region コンストラクタ・デストラクタ

FollowCameraComponent::FollowCameraComponent(GameObject& parent, int updateLayer)
    : CameraComponent(parent, updateLayer)
    , _xOffset(0.0f)
    , _yOffset(0.0f)
    , _attenRate(3.0f)
    , _internalPosition(Vector2::Zero) {
    // 初期化時に現在位置を設定
    
}

FollowCameraComponent::~FollowCameraComponent() {
    // デストラクタの処理
}

#pragma endregion

#pragma region パブリック関数

void FollowCameraComponent::Update(float deltaTime) {
 
}

void FollowCameraComponent::LateUpdate(float deltaTime) {
	// ターゲットがnullptrの場合は何もしない

	if (_targeObject == nullptr) {
		return;
	}

    // 前回の位置を保存
    _prevInternalPosition = _internalPosition;

    // ターゲットにオフセットを加えた目標位置を計算
    Vector2 targetPos(_targeObject->Position().x + _xOffset, _targeObject->Position().y + _yOffset);

    // SmoothDampを使って滑らかに目標位置に近づける
    _internalPosition = Vector2::SmoothDamp(_internalPosition, targetPos, _currentVelocity,
        0.5f, // smoothTime - 値が小さいほど速く目標に近づく
        1000.0f, // maxSpeed - 最大移動速度
        deltaTime);

	// SDL_Log("Internal Position: %f, %f", _internalPosition.x, _internalPosition.y);
    // GameObject の位置を更新
    _parent.Position2D(_internalPosition);
}

#pragma endregion

#pragma region プライベート関数

#pragma endregion