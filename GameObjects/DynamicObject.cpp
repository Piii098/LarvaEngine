#include "GameObjects/DynamicObject.h"

#pragma region コンストラクタ

DynamicObject::DynamicObject(Game* game)
	: GameObject(game)
	, _mass(0.f)
	, _sumOfForces(Vector2::Zero)
	, _velocity(Vector2::Zero){

}

DynamicObject::~DynamicObject() {

}

#pragma endregion

#pragma region パブリック関数



#pragma endregion
