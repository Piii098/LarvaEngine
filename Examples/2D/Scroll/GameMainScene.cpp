#include "Examples/2D/Scroll/GameMainScene.h"
#include "Examples/2D/Scroll/Background.h"
#include "Examples/2D/Scroll/Player.h"
#include "Examples/2D/Scroll/TileMap.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Core/Resources/TileMap.h"
#include "Examples/2D/Scroll/PlaySubScene.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Components/Camera/FollowCameraComponent.h"
#include "LarvaEngine/Input/InputAction.h"
#include "Examples/2D/Scroll/MenuUIScene.h"
#include "Examples/2D/Scroll/TitleUIScene.h"
#include "Examples/2D/Scroll/HUDUIScene.h"

Example2DScroll::GameMainScene::GameMainScene(SceneManager& manager)
	: MainScene(manager)
	, _isMenu(false){

}

Example2DScroll::GameMainScene::~GameMainScene() {

}

void Example2DScroll::GameMainScene::InputScene(const InputAction& input) {
	MainScene::InputScene(input); // 親クラスの入力処理を呼び出す
}

void Example2DScroll::GameMainScene::UpdateScene(float deltaTime) {
	MainScene::UpdateScene(deltaTime); // 親クラスの更新処理を呼び出す
	
	SDL_Log("Camera Position: %f, %f", _camera->Position().x, _camera->Position().y); // 確認用ログ
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
	_manager.GetGame().GetTextureManager().Load("PlayerBar", "Assets/Textures/playerBar.png");
	_manager.GetGame().GetTextureManager().Load("TileTexture_0", "Assets/Textures/16Tile.png");
	_manager.GetGame().GetTileMapManager().Load("TileMap_0", "Assets/TileMaps/TileMap_0.csv");

}

void Example2DScroll::GameMainScene::LoadObjects() {

	Player& player = CreateGameObject<Player>();

	GameObject& child = CreateChildObject<GameObject>(&player);
	SpriteComponent& sprite = child.CreateComponent<SpriteComponent>(10);
	sprite.SetTexture("Rectangle");
	sprite.TexScale(Vector2(16, 16));
	child.Position(Vector3(0, 20, 0));
	auto& tileMap = CreateGameObject<TileMap>();
	tileMap.Position2D(-100, -100);
	tileMap.SetOnTile(64, &player);

	CreateGameObject<Background>();

	_camera = &CreateGameObject<Camera>();

	FollowCameraComponent& followCameraComp = _camera->CreateComponent<FollowCameraComponent>();
	followCameraComp.SetTargetObject(&player);
	_camera->SetCameraComponent(&followCameraComp);

	_camera->Position(player.Position());
	ChangeSubScene<PlaySubScene>(player);
	_currentSubScene->Initialize();

	CreateUIScene<HUDUIScene>();

}