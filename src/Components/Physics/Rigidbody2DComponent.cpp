#include "LarvaEngine/Components/Physics/Rigidbody2DComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/FrameSystem.h"
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"

Rigidbody2DComponent::Rigidbody2DComponent(GameObject& parent, int updateLayer)
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
    

    _parent.GetScene().GetManager().GetGame().GetPhysWorld2D().AddRigidbodyComponent(this);  // Rigidbodyを物理ワールドに追加  

}

Rigidbody2DComponent::~Rigidbody2DComponent() {
    _parent.GetScene().GetManager().GetGame().GetPhysWorld2D().RemoveRigidbodyComponent(this);  // Rigidbodyを物理ワールドから削除
}


// 更新処理で補間を適用
void Rigidbody2DComponent::Update(float deltaTime) {

}

void Rigidbody2DComponent::FixedUpdate(float deltaTime) {

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
    auto& physWorld = _parent.GetScene().GetManager().GetGame().GetPhysWorld2D();

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

void Rigidbody2DComponent::CalculateVelocity(float deltaTime) {
    if (_mass <= 0.f) return;

    _internalPosition = Vector2(_parent.Position().x, _parent.Position().y);  // 親オブジェクトの位置を取得

    Vector2 totalForce = _sumOfForces - (_velocity * _drag);
    Vector2 accele;
    accele.x = totalForce.x / _mass;
    accele.y = totalForce.y / _mass;

    _velocity += accele * deltaTime;
    
    _internalPosition += _velocity * deltaTime;

	//SDL_Log("Velocity: %f, %f", _velocity.x, _velocity.y);

	_sumOfForces = Vector2::Zero; // 力をリセット
     

}

void Rigidbody2DComponent::UpdatePosition(float deltaTime) {
  
	 _parent.Position2D(_internalPosition);
     _internalPosition = Vector2(_parent.Position().x, _parent.Position().y);  // 親オブジェクトの位置を取得
 
}

void Rigidbody2DComponent::AddForce(Vector2 force) {
    _sumOfForces += force;
}

#pragma endregion