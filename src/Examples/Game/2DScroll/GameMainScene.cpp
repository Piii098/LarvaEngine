#include "LarvaEngine/Examples/Game/2DScroll/GameMainScene.h"
#include "LarvaEngine/Examples/Game/2DScroll/Player.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Examples/Game/2DScroll/PlaySubScene.h"


Example2DScroll::GameMainScene::GameMainScene(SceneManager& manager)
	: MainScene(manager){

}

Example2DScroll::GameMainScene::~GameMainScene() {

}

void Example2DScroll::GameMainScene::LoadData() {

	_manager.GetGame().GetTextureManager().Load("Player", "Assets/Textures/16Player.png");

	
	_player = &CreateGameObject<Player>();
	_camera = &CreateGameObject<Camera>();

	ChangeSubScene<PlaySubScene>(*_player);
	_currentSubScene->Initialize();
}