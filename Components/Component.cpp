#include "Components/Component.h"
#include "GameObjects/GameObject.h"

Component::Component(GameObject* parent,int updateLayer) 
	: _parent(parent) 
	, _updateLayer(updateLayer){
	_parent->AddComponent(this);
}

Component::~Component() {

}

#pragma region �p�u���b�N�֐�

void Component::ProcessInput(Input* input) {

}

void Component::Update(Frame* frame) {

}


#pragma endregion