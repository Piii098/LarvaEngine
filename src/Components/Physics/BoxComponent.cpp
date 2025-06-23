#include "LarvaEngine/Components/Physics/BoxComponent.h"

BoxComponent::BoxComponent(GameObject& parent, bool isCollision, bool isDynamic, int updateLayer)
    : Component(parent, updateLayer), _isCollision(isCollision), _isDynamic(isDynamic) {
}

BoxComponent::~BoxComponent() {
}