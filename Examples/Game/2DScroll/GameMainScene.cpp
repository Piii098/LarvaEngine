#include "Examples/Game/2DScroll/GameMainScene.h"
#include "Examples/Game/2DScroll/Background.h"
#include "Examples/Game/2DScroll/Player.h"
#include "Examples/Game/2DScroll/TileMap.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Core/Resources/TileMap.h"
#include "Examples/Game/2DScroll/PlaySubScene.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Components/Camera/FollowCameraComponent.h"


Example2DScroll::GameMainScene::GameMainScene(SceneManager& manager)
	: MainScene(manager){

}

Example2DScroll::GameMainScene::~GameMainScene() {

}

void Example2DScroll::GameMainScene::UpdateScene(float deltaTime) {
	
	//SDL_Log("GameMainScene Camera Position: %d, %d", _camera->Position().x, _camera->Position().y); // 確認用ログ
}

void Example2DScroll::GameMainScene::LoadData() {

	_manager.GetGame().GetTextureManager().Load("Player", "Assets/Textures/16Player.png");
	_manager.GetGame().GetTextureManager().Load("BG1", "Assets/Textures/bg_1.png");
	_manager.GetGame().GetTextureManager().Load("BG2", "Assets/Textures/bg_2.png");
	_manager.GetGame().GetTextureManager().Load("BG3", "Assets/Textures/bg_3.png");
	_manager.GetGame().GetTextureManager().Load("BG4", "Assets/Textures/bg_4.png");
	_manager.GetGame().GetTextureManager().Load("BG5", "Assets/Textures/bg_5.png");
	_manager.GetGame().GetTextureManager().Load("BG6", "Assets/Textures/bg_6.png");
	_manager.GetGame().GetTextureManager().Load("TileTexture_0", "Assets/Textures/16Tile.png");
	_manager.GetGame().GetTileMapManager().Load("TileMap_0", "Assets/TileMaps/TileMap_0.csv");

}

void Example2DScroll::GameMainScene::LoadObjects() {

	Player& player = CreateGameObject<Player>();

	//GameObject& child = CreateChildObject<GameObject>(&player);
	//SpriteComponent& sprite = child.CreateComponent<SpriteComponent>(10);
	//sprite.SetTexture("Rectangle");
	//child.Scale(0.25f);
	//child.Position(Vector2Int(0, 20));
	CreateGameObject<TileMap>();

	CreateGameObject<Background>();

	_camera = &CreateGameObject<Camera>();

	FollowCameraComponent& followCameraComp = _camera->CreateComponent<FollowCameraComponent>();
	followCameraComp.SetTargetObject(&player);
	_camera->SetCameraComponent(&followCameraComp);

	//_camera->Position(Vector2Int(0, 700));
	ChangeSubScene<PlaySubScene>(player);
	_currentSubScene->Initialize();
}