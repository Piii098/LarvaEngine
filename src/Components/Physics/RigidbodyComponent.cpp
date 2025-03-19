#include "LarvaEngine/Components/Physics/RigidbodyComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/Frame.h"
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"

RigidbodyComponent::RigidbodyComponent(GameObject* parent, int updateLayer)
    : Component(parent, updateLayer)
    , _angularSpeed(0.f)
    , _forwardSpeed(0.f)
    , _mass(1.f) // 質量をゼロにしない
    , _drag(0.f)
    , _sumOfForces(Vector2::Zero)
    , _velocity(Vector2::Zero)
    , _isGravity(false)
    , _internalPosition(Vector2::Zero)
    , _prevInternalPosition(Vector2::Zero)  // 前回位置の初期化
    , _interpolationMode(InterpolationMode::None) {  // デフォルトは補間なし
    //_internalPosition = _parent->PositionToFloat();
    _phys = _parent->GetScene()->GetManager()->GetGame()->GetPhysWorld();
    _parent->GetScene()->GetManager()->GetGame()->GetPhysWorld()->AddRigidbodyComp(this);
}

RigidbodyComponent::~RigidbodyComponent() {
    _parent->GetScene()->GetManager()->GetGame()->GetPhysWorld()->RemoveRigidbodyComp(this);
}

#pragma region �p�u���b�N�֐�

// 位置設定時に前回の位置も更新するように変更
void RigidbodyComponent::InternalPosition(Vector2 internalPosition) {
    _prevInternalPosition = _internalPosition;  // 現在の位置を前回の位置として保存
    _internalPosition = internalPosition;  // 新しい位置を設定
}

// 更新処理で補間を適用
void RigidbodyComponent::Update(float deltaTime) {
    if (_interpolationMode == InterpolationMode::None) {
        // 補間なしの場合は物理計算の位置をそのまま使用
        _parent->Position(_internalPosition);
        return;
    }

    // 補間モードの場合、前回と現在の位置を補間
    float alpha = GetParent()->GetScene()->GetManager()->GetGame()->GetFrame()->GetAlpha();  // Frame クラスから補間係数を取得
    Vector2 interpolatedPosition = Vector2::Lerp(_prevInternalPosition, _internalPosition, alpha);

    // 補間位置をゲームオブジェクトに適用
    _parent->Position(interpolatedPosition);
}


void RigidbodyComponent::PhysUpdate(float deltaTime) {

    if (_internalPosition == Vector2::Zero) {
        _internalPosition = _parent->PositionToFloat();
        return;
    }


    const float GRAVITY_CONSTANT = 50000; // 重力定数を現実的な値に修正
   
    float gravityForce = 0.f;

    if (_isGravity) {
        PhysWorld2D::CollisionInfo outColl;


        // プレイヤーの少し下から地面方向へレイを飛ばす
        Vector2Int rayStart = _parent->Position() + Vector2Int(0.f, -1.0f);  // プレイヤーの足元あたり
        LineSegment2D ray(rayStart, rayStart + Vector2Int(0.f, -15));


        if (!_phys->SegmentCast(ray, outColl, GetParent())) {
            gravityForce = _mass * GRAVITY_CONSTANT * deltaTime;
         
        }
        
        AddForce(Vector2(0, -gravityForce));
    }
    
    
    CalculateVelocity(deltaTime);
    UpdatePosition(deltaTime);
}
void RigidbodyComponent::CalculateVelocity(float deltaTime) {
    if (_mass <= 0.f) return;

    Vector2 accele;
    accele.x = _sumOfForces.x / _mass;
    accele.y = _sumOfForces.y / _mass;

    _velocity += accele * deltaTime;

    _internalPosition += _velocity * deltaTime;

    // デバッグログ
   // SDL_Log("PlayerVelocity(%.6f, %.6f)", _velocity.x, _velocity.y);
   // SDL_Log("PlayerInternalPos(%.6f, %.6f)", _internalPosition.x, _internalPosition.y);

    // 力のリセット
    _sumOfForces = Vector2::Zero;
}

void RigidbodyComponent::UpdatePosition(float deltaTime) {
  
    _parent->Position(_internalPosition);
 
}

void RigidbodyComponent::AddForce(Vector2 force) {
    _sumOfForces += force;
}

#pragma endregion