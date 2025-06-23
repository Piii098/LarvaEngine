#include "LarvaEngine/Components/Physics/Box3DComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Physics/PhysWorld3D.h"
#include "LarvaEngine/Renderer/Renderer.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"

Box3DComponent::Box3DComponent(GameObject& parent, bool isCollision, bool isDynamic, int updateLayer)
    : Component(parent, updateLayer)
    , _objectBox(Vector3::Zero, Vector3::Zero)
    , _worldBox(Vector3::Zero, Vector3::Zero) {
    //_parent.GetScene().GetManager().GetGame().GetPhysWorld3D().AddBoxComponent(this);
}

Box3DComponent::~Box3DComponent() {
    //_parent.GetScene().GetManager().GetGame().GetPhysWorld3D().RemoveBoxComponent(this);
}

void Box3DComponent::OnUpdateWorldTransform() {
    const Matrix4& worldTransform = _parent.WorldTransform();

    Vector3 objectMin = _objectBox._min;
    Vector3 objectMax = _objectBox._max;

    objectMin = Vector3::Transform(objectMin, worldTransform);
    objectMax = Vector3::Transform(objectMax, worldTransform);

    _worldBox._min = objectMin;
    _worldBox._max = objectMax;
}

void Box3DComponent::DisplayBox(const Vector3& color) const {
  
}