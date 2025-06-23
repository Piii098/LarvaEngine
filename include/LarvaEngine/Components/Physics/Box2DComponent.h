#pragma once
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Physics/Collision2D.h"

class Box2DComponent : public Component {
public:
	Box2DComponent(GameObject& parent, bool isCollision, bool isDynamic, int updateLayer = 10);
	~Box2DComponent();
	void OnUpdateWorldTransform() override;
	
	void SetObjectBox(const AABB2D& model) { _objectBox = model; };
	const AABB2D& GetWorldBox() const { return _worldBox; };

	void DisplayBox(const Vector3& color = Color::Red) const;

	Vector2 ObjectMax() const { return _objectBox._max; };
	Vector2 ObjectMin() const { return _objectBox._min; };
	Vector2 WorldMax() const { return _worldBox._max; };
	Vector2 WorldMin() const { return _worldBox._min; };

	bool IsCollision() const { return _isCollision; };
	bool IsDynamic() const { return _isDynamic; };
	void IsCollision(bool flag) { _isCollision = flag; };
	void IsDynamic(bool flag) { _isDynamic = flag; };

private:
	AABB2D _objectBox;
	AABB2D _worldBox;

	bool _isCollision;
	bool _isDynamic;
};