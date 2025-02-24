#include "GameObjects/Background.h"
#include "Components/BGComponent.h"

#pragma region コンストラクタ:デストラクタ

Background::Background(Game* game) 
	: GameObject(game){
	
	_bgComp = new BGComponent(this);
	_bgComp->SetTexture("Assets/BG.png");
}

Background::~Background() {

}

#pragma endregion

#pragma region パブリック関数


#pragma endregion
