#pragma once
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Physics/Collision2D.h"

class BoxComponent2D : public Component {
public:
	BoxComponent2D(GameObject* parent, bool isCollision, bool isDynamic, int updateLayer = 10);
	~BoxComponent2D();
	void OnUpdateWorldTransform() override;
	
	void SetObjectBox(const AABB2D& model) { _objectBox = model; };
	const AABB2D& GetWorldBox() const { return _worldBox; };

	Vector2Int ObjectMax() { return _objectBox._max; };
	Vector2Int ObjectMin() { return _objectBox._min; };
	Vector2Int WorldMax() { return _worldBox._max; };
	Vector2Int WorldMin() { return _worldBox._min; };

	bool IsCollision() { return _isCollision; };
	bool IsDynamic() { return _isDynamic; };
	void IsCollision(bool flag) { _isCollision = flag; };
	void IsDynamic(bool flag) { _isDynamic = flag; };

private:
	AABB2D _objectBox;
	AABB2D _worldBox;

	bool _isCollision;
	bool _isDynamic;
};