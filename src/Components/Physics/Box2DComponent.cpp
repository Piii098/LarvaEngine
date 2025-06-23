#include "LarvaEngine/Components/Physics/Box2DComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"

#pragma region コンストラクタ:デストラクタ

Box2DComponent::Box2DComponent(GameObject& parent, bool isCollision, bool isDynamic, int updateLayer)
	: Component(parent)
	, _objectBox(Vector2::Zero, Vector2::Zero)
	, _worldBox(Vector2::Zero, Vector2::Zero)
	, _isCollision(isCollision)
	, _isDynamic(isDynamic){

	_parent.GetScene().GetManager().GetGame().GetPhysWorld2D().AddBoxComponent(this);

}

Box2DComponent::~Box2DComponent() {
	_parent.GetScene().GetManager().GetGame().GetPhysWorld2D().RemoveBoxComponent(this);
}

#pragma endregion

#pragma region パブリック関数

void Box2DComponent::DisplayBox(const Vector3& color) const {

	auto& spriComp = _parent.CreateComponent<SpriteComponent>(10);
	spriComp.SetTexture("Rectangle");
	Vector2 size = Vector2(_objectBox._max * 2);
	spriComp.TexScale(size);
	spriComp.Alpha(0.5f);
	spriComp.Color(color);

}

void Box2DComponent::OnUpdateWorldTransform() {
	const Matrix4& worldTransform = _parent.WorldTransform();

	Vector3 objectMin = Vector3(_objectBox._min.x, _objectBox._min.y, 0);
	Vector3 objectMax = Vector3(_objectBox._max.x, _objectBox._max.y, 0);

	objectMin = Vector3::Transform(objectMin, worldTransform);
	objectMax = Vector3::Transform(objectMax, worldTransform);

	_worldBox._min = Vector2(objectMin.x, objectMin.y);
	_worldBox._max = Vector2(objectMax.x, objectMax.y);
}


#pragma endregion
