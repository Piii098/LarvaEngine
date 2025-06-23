#include "LarvaEngine/Components/Physics/Rigidbody3DComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/FrameSystem.h"
#include "LarvaEngine/Physics/PhysWorld3D.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"

Rigidbody3DComponent::Rigidbody3DComponent(GameObject& parent, int updateLayer)
    : Component(parent, updateLayer)
    , _angularSpeed(0.f)
    , _forwardSpeed(0.f)
    , _mass(1.f) // 質量をゼロにしない
    , _drag(0)
    , _sumOfForces(Vector3::Zero)
    , _velocity(Vector3::Zero)
    , _isGravity(false)
    , _onGround(false) // 地面に接触していない状態で初期化
    , _internalPosition(Vector3::Zero) {


    _parent.GetScene().GetManager().GetGame().GetPhysWorld3D().AddRigidbodyComponent(this);  // Rigidbodyを物理ワールドに追加  

}

Rigidbody3DComponent::~Rigidbody3DComponent() {
    _parent.GetScene().GetManager().GetGame().GetPhysWorld3D().RemoveRigidbodyComponent(this);  // Rigidbodyを物理ワールドから削除
}


// 更新処理で補間を適用
void Rigidbody3DComponent::Update(float deltaTime) {

}

void Rigidbody3DComponent::FixedUpdate(float deltaTime) {

    // 毎フレームで初期化
    _onGround = false;



    // 重力を適用
    if (_isGravity && !_onGround) {
        const float GRAVITY_CONSTANT = 4000.0f;
        AddForce(Vector3(0, 0, -GRAVITY_CONSTANT));
    }
}

void Rigidbody3DComponent::CalculateVelocity(float deltaTime) {
    if (_mass <= 0.f) return;

    _internalPosition = _parent.Position();  // 親オブジェクトの位置を取得

    Vector3 totalForce = _sumOfForces - (_velocity * _drag);
    Vector3 accele;
    accele.x = totalForce.x / _mass;
    accele.y = totalForce.y / _mass;
	accele.z = totalForce.z / _mass;

    _velocity += accele * deltaTime;

    _internalPosition += _velocity * deltaTime;

    //SDL_Log("Velocity: %f, %f", _velocity.x, _velocity.y);

    _sumOfForces = Vector2::Zero; // 力をリセット


}

void Rigidbody3DComponent::UpdatePosition(float deltaTime) {

    _parent.Position(_internalPosition);

    // 内部位置を更新
    _internalPosition = _parent.Position();

}

void Rigidbody3DComponent::Move(float forward, float strafe) {
    Vector3 forardDir = _parent.GetForward();
	Vector3 rightDir = _parent.GetRight();
	_velocity = (forardDir * forward) + (rightDir * -strafe);
}

void Rigidbody3DComponent::MoveRotate(const Vector3& axis, float angularSpeed) {
	
}

void Rigidbody3DComponent::AddForce(Vector3 force) {
    _sumOfForces += force;
}

#pragma endregion