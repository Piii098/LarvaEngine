#include "GameObjects/Background.h"
#include "Components/Draw/SpriteComponent.h"
#include "Utilities/Math.h"

#pragma region コンストラクタ:デストラクタ

Background::Background(Scene* scene)
	: GameObject(scene){

	Position(Vector2Int(0, 0));
	Scale(1.f);
	SpriteComponent* bg0 = new SpriteComponent(this, 0, 10);
	bg0->SetTexture("Bg0");
	SpriteComponent* bg1 = new SpriteComponent(this, 1, 10);
	bg1->SetTexture("Bg1");
	SpriteComponent* bg2 = new SpriteComponent(this, 2, 10);
	bg2->SetTexture("Bg2");
	SpriteComponent* bg3 = new SpriteComponent(this, 3, 10);
	bg3->SetTexture("Bg3");
	SpriteComponent* bg4 = new SpriteComponent(this, 4, 10);
	bg4->SetTexture("Bg4");
	SpriteComponent* bg5 = new SpriteComponent(this, 5, 10);
	bg5->SetTexture("Bg5");
	SpriteComponent* bg6 = new SpriteComponent(this, 14, 10);
	bg6->SetTexture("Bg6");

	//_sky->SelfLightColor(Vector3::fromIntRGB(255, 255, 255));
	//_sky->SelfLightIntensity(1.1);
	//SpriteComponent* _mountain = new SpriteComponent(this, 1, 15);
	//_mountain->SetTexture("Mountain");
	//_mountain->SelfLightIntensity(0.9);
	//SpriteComponent* _plain = new SpriteComponent(this, 2, 20);
	//_plain->SetTexture("Plain");
	//_plain->SelfLightIntensity(0.8);

	_bgComps.emplace_back(bg0);
	_bgComps.emplace_back(bg1);
	_bgComps.emplace_back(bg2);
	_bgComps.emplace_back(bg3);
	_bgComps.emplace_back(bg4);
	_bgComps.emplace_back(bg5);
}

Background::~Background() {
	for (auto i : _bgComps) {
		delete i;
		i = nullptr;
	}
	_bgComps.clear();
}

#pragma endregion

#pragma region パブリック関数


#pragma endregion
