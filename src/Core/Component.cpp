#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/GameObject.h"

Component::Component(GameObject* parent,int updateLayer) 
	: _parent(parent)
	, _state(STATE::ACTIVE)
	, _bufferLayer(0)
	, _renderType(0)
	, _updateLayer(updateLayer){
	_parent->AddComponent(this);
}

Component::~Component() {
	_parent->RemoveComponent(this);
}

#pragma region パブリック関数


#pragma endregion