#include "Components/Component.h"
#include "GameObjects/GameObject.h"

Component::Component(GameObject* parent,int updateLayer) 
	: _parent(parent) 
	, _updateLayer(updateLayer){
	_parent->AddComponent(this);
}

Component::~Component() {
	_parent->RemoveComponent(this);
}

#pragma region パブリック関数

void Component::ProcessInput(Input* input) {

}

void Component::Update(Frame* frame) {

}

void Component::PhysUpdate(float deltaTime){

}

#pragma endregion