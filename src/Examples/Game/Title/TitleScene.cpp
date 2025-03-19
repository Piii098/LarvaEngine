#include "LarvaEngine/Examples/Game/Title/TitleScene.h"
#include "LarvaEngine/Examples/Game/Title/PlaySubScene.h"

#pragma region コンストラクタデストラクタ

ExampleTitle::TitleScene::TitleScene(SceneManager* manager)
	: MainScene(manager){
}

ExampleTitle::TitleScene::~TitleScene(){
}

#pragma endregion

#pragma region パブリック関数



#pragma endregion

#pragma region プライベート関数

void ExampleTitle::TitleScene::LoadData() {
	_currentSubScene = new PlaySubScene(this);
	_currentSubScene->Initialize();
}

#pragma endregion
