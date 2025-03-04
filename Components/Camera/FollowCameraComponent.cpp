#include "Components/Camera/FollowCameraComponent.h"
#include "GameObjects/GameObject.h"
#include "Utilities/Frame.h"

#pragma region シングルトン関連

#pragma endregion


#pragma region コンストラクタ・デストラクタ

FollowCameraComponent::FollowCameraComponent(GameObject* parent, int updateLayer)
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

void FollowCameraComponent::Update(Frame* frame) {
 
}

void FollowCameraComponent::PhysUpdate(float deltaTime) {
	if (_internalPosition == Vector2::Zero) {
		_internalPosition = _parent->PositionToFloat();
		return;
	}
    // 前回の位置を保存
    _prevInternalPosition = _internalPosition;

    // ターゲットにオフセットを加えた目標位置を計算
    Vector2 targetPos(_target.x + _xOffset, _target.y + _yOffset);

    // SmoothDampを使って滑らかに目標位置に近づける
    _internalPosition = Vector2::SmoothDamp(_internalPosition, targetPos, _currentVelocity,
        0.15f, // smoothTime - 値が小さいほど速く目標に近づく
        500.0f, // maxSpeed - 最大移動速度
        deltaTime);

    // GameObject の位置を更新
    _parent->Position(_internalPosition);
}

#pragma endregion

#pragma region プライベート関数

#pragma endregion