#include "LarvaEngine/Components/Physics/RigidbodyComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/FrameSystem.h"
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"

RigidbodyComponent::RigidbodyComponent(GameObject& parent, int updateLayer)
    : Component(parent, updateLayer)
    , _angularSpeed(0.f)
    , _forwardSpeed(0.f)
    , _mass(1.f) // 質量をゼロにしない
	, _drag(0)
    , _sumOfForces(Vector2::Zero)
    , _velocity(Vector2::Zero)
    , _isGravity(false)
	, _onGround(false) // 地面に接触していない状態で初期化
    , _internalPosition(Vector2::Zero){  
    
    //_internalPosition = _parent->PositionToFloat();
    _parent.GetScene().GetManager().GetGame().GetPhysWorld().AddRigidbodyComponent(this);  // Rigidbodyを物理ワールドに追加  

}

RigidbodyComponent::~RigidbodyComponent() {
    _parent.GetScene().GetManager().GetGame().GetPhysWorld().RemoveRigidbodyComponent(this);  // Rigidbodyを物理ワールドから削除
}


// 更新処理で補間を適用
void RigidbodyComponent::Update(float deltaTime) {

}

void RigidbodyComponent::FixedUpdate(float deltaTime) {

    // 毎フレームで初期化
    _onGround = false;

    // 地面との接触を確認する
    LineSegment2D ray(
        _internalPosition + Vector2(0, -8),
        _internalPosition + Vector2(0, -8) + Vector2(0, -0.5) // 下方向に短い線分を作成
    );

    // SDL_Log("raystart: %f, %f", ray._start.x, ray._start.y);
    // SDL_Log("rayend: %f, %f", ray._end.x, ray._end.y);

    CollisionInfo collInfo;
    auto& physWorld = _parent.GetScene().GetManager().GetGame().GetPhysWorld();

    if (physWorld.SegmentCast(ray, collInfo, _parent)) {
        // 接触したオブジェクトがGROUNDタグを持つ場合
        if (collInfo._object->Tag() == GameObject::TAG::GROUND) {
            // SDL_Log("Ground");
            _onGround = true; // 地面に接触しているとマーク
        }
    }

    // 重力を適用
    if (_isGravity && !_onGround) {
        const float GRAVITY_CONSTANT = 4000.0f;
        AddForce(Vector2(0, -GRAVITY_CONSTANT ));
    }
}

void RigidbodyComponent::CalculateVelocity(float deltaTime) {
    if (_mass <= 0.f) return;

    _internalPosition = _parent.PositionToFloat();  // 親オブジェクトの位置を取得

    Vector2 totalForce = _sumOfForces - (_velocity * _drag);
    Vector2 accele;
    accele.x = totalForce.x / _mass;
    accele.y = totalForce.y / _mass;

    _velocity += accele * deltaTime;
    
    _internalPosition += _velocity * deltaTime;

	//SDL_Log("Velocity: %f, %f", _velocity.x, _velocity.y);

	_sumOfForces = Vector2::Zero; // 力をリセット
     

}

void RigidbodyComponent::UpdatePosition(float deltaTime) {
  
	 _parent.Position(_internalPosition);
     _internalPosition = _parent.PositionToFloat();  // 親オブジェクトの位置を取得
 
}

void RigidbodyComponent::AddForce(Vector2 force) {
    _sumOfForces += force;
}

#pragma endregion