#pragma once

#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Physics/Collision3D.h"

class Box3DComponent : public Component {
public:
    Box3DComponent(GameObject& parent, bool isCollision, bool isDynamic, int updateLayer = 10);
    ~Box3DComponent();

    void OnUpdateWorldTransform();
    void DisplayBox(const Vector3& color = Color::Red) const;

    void SetObjectBox(const AABB3D& model) { _objectBox = model; };
    const AABB3D& GetWorldBox() const { return _worldBox; };

    Vector3 ObjectMax() const { return _objectBox._max; };
    Vector3 ObjectMin() const { return _objectBox._min; };
    Vector3 WorldMax() const { return _worldBox._max; };
    Vector3 WorldMin() const { return _worldBox._min; };

    bool IsCollision() const { return _isCollision; };
    bool IsDynamic() const { return _isDynamic; };
    void IsCollision(bool flag) { _isCollision = flag; };
    void IsDynamic(bool flag) { _isDynamic = flag; };

private:
    AABB3D _objectBox;
    AABB3D _worldBox;

    bool _isCollision;
    bool _isDynamic;
};