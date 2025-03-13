#include "Scene/Game/Title/TitleScene.h"
#include "Scene/Game/Title/PlaySubScene.h"

#pragma region コンストラクタデストラクタ

TitleScene::TitleScene(SceneManager* manager)
	: MainScene(manager){
}

TitleScene::~TitleScene(){
}

#pragma endregion

#pragma region パブリック関数



#pragma endregion

#pragma region プライベート関数

void TitleScene::LoadData() {
	_currentSubScene = new Title::PlaySubScene(this);
	_currentSubScene->Initialize();
}

#pragma endregion
