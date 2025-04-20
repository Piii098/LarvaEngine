#include "LarvaEngine/GameObjects/Background.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Core/Utilities/Math.h"

#pragma region コンストラクタ:デストラクタ

Background::Background(Scene& scene)
	: GameObject(scene){

	Position(Vector2Int(0, 0));
	Scale(1.f);
	SpriteComponent* bg0 = CreateComponent<SpriteComponent>(1, 1);
	bg0->SetTexture("Bg1");
	SpriteComponent* bg1 = CreateComponent<SpriteComponent>(2, 1);
	bg1->SetTexture("Bg2");
	SpriteComponent* bg2 = CreateComponent<SpriteComponent>(3, 1);
	bg2->SetTexture("Bg3");
	SpriteComponent* bg3 = CreateComponent<SpriteComponent>(4, 1);
	bg3->SetTexture("Bg4");
	SpriteComponent* bg4 = CreateComponent<SpriteComponent>(14, 1);
	bg4->SetTexture("Bg5");

}

Background::~Background() {

}

#pragma endregion

#pragma region パブリック関数


#pragma endregion
