#include "GameObjects/TestObject.h"
#include "Components/BoxComponent2D.h"
#include "Components/Debug/DebugDrawComponent.h"
#include "Components/SpriteComponent.h"
#include "PhysWorld2D.h"
#include "Game.h"
#include "Components/RigidbodyComponent.h"

#pragma region コンストラクタ:デストラクタ

TestObject::TestObject(Game* game)
	: GameObject(game){
	Scale(1);
	SpriteComponent* spri = new SpriteComponent(this);
	spri->SetTexture("Assets/Tama.png");
	spri->SelfLightColor(Vector3::fromIntRGB(255,232,87));
	spri->SelfLightIntensity(2);
	DynamicComponent* dyna = new DynamicComponent(this, false);
	dyna->rigidbodyComp->Mass(10.f);
	dyna->rigidbodyComp->Velocity(Vector2(10, 0));
}

TestObject::~TestObject() {

}

#pragma endregion

#pragma region パブリック関数



#pragma endregion
