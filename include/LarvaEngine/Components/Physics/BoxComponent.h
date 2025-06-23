#pragma once

#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/Utilities/Math.h"

class BoxComponent : public Component {
public:
    BoxComponent(GameObject& parent, bool isCollision, bool isDynamic, int updateLayer = 10);
    virtual ~BoxComponent();

    virtual void OnUpdateWorldTransform() override = 0; // 純粋仮想関数（派生クラスで実装）
    virtual void DisplayBox(const Vector3& color = Color::Red) const = 0;

    bool IsCollision() const { return _isCollision; };
    bool IsDynamic() const { return _isDynamic; };
    void IsCollision(bool flag) { _isCollision = flag; };
    void IsDynamic(bool flag) { _isDynamic = flag; };

protected:
    bool _isCollision;
    bool _isDynamic;
};