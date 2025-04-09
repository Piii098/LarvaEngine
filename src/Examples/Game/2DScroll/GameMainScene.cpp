#include "LarvaEngine/Examples/Game/2DScroll/GameMainScene.h"
#include "LarvaEngine/Core/GameObject.h"

Example2DScroll::GameMainScene::GameMainScene(SceneManager& manager)
	: MainScene(manager){

}

Example2DScroll::GameMainScene::~GameMainScene() {

}

void Example2DScroll::GameMainScene::LoadData() {
	_player = &CreateGameObject<GameObject>();
	
}