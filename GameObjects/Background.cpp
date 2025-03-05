#include "GameObjects/Background.h"
#include "Components/Draw/BGComponent.h"
#include "Utilities/Math.h"

#pragma region コンストラクタ:デストラクタ

Background::Background(Scene* scene)
	: GameObject(scene){
	
	BGComponent* _sky = new BGComponent(this, 1, 10);
	_sky->SetTexture("Sky");
	_sky->SelfLightColor(Vector3::fromIntRGB(255, 255, 255));
	_sky->SelfLightIntensity(1.1);
	BGComponent* _mountain = new BGComponent(this, 0.95, 15);
	_mountain->SetTexture("Mountain");
	_mountain->SelfLightIntensity(0.9);
	BGComponent* _plain = new BGComponent(this, 0.9, 20);
	_plain->SetTexture("Plain");
	_plain->SelfLightIntensity(0.8);

	_bgComps.emplace_back(_sky);
	_bgComps.emplace_back(_mountain);
	_bgComps.emplace_back(_plain);
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
