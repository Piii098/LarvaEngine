#include "GameObjects/Background.h"
#include "Components/BGComponent.h"

#pragma region �R���X�g���N�^:�f�X�g���N�^

Background::Background(Game* game) 
	: GameObject(game){
	
	_bgComp = new BGComponent(this);
	_bgComp->SetTexture("Assets/BG.png");
}

Background::~Background() {

}

#pragma endregion

#pragma region �p�u���b�N�֐�


#pragma endregion
