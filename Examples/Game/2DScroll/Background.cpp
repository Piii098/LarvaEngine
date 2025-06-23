#include "Examples/Game/2DScroll/Background.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Core/Utilities/Math.h"

#pragma region コンストラクタ:デストラクタ

Example2DScroll::Background::Background(Scene& scene)
	: GameObject(scene){

	Position2D(Vector2(0, 0));
	Scale(1.f);
	SpriteComponent& bg1 = CreateComponent<SpriteComponent>(0, 1);
	bg1.SetTexture("BG1");
	SpriteComponent& bg2 = CreateComponent<SpriteComponent>(2, 1);
	bg2.SetTexture("BG2");
	SpriteComponent& bg3 = CreateComponent<SpriteComponent>(3, 1);
	bg3.SetTexture("BG3");
	SpriteComponent& bg4 = CreateComponent<SpriteComponent>(4, 1);
	bg4.SetTexture("BG4");
	SpriteComponent& bg5 = CreateComponent<SpriteComponent>(4, 1);
	bg5.SetTexture("BG5");
	SpriteComponent& bg6 = CreateComponent<SpriteComponent>(14, 1);
	bg6.SetTexture("BG6");

}

Example2DScroll::Background::~Background() {

}

#pragma endregion

#pragma region パブリック関数


#pragma endregion
