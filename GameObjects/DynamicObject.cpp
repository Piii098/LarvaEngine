#include "GameObjects/DynamicObject.h"

#pragma region �R���X�g���N�^

DynamicObject::DynamicObject(Game* game)
	: GameObject(game)
	, _mass(0.f)
	, _sumOfForces(Vector2::Zero)
	, _velocity(Vector2::Zero){

}

DynamicObject::~DynamicObject() {

}

#pragma endregion

#pragma region �p�u���b�N�֐�



#pragma endregion
