#include "Examples/Game/2DScroll/GameMainScene.h"

#include "Examples/Game/2DScroll/Player.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "Examples/Game/2DScroll/PlaySubScene.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Components/Camera/FollowCameraComponent.h"

Example2DScroll::GameMainScene::GameMainScene(SceneManager& manager)
	: MainScene(manager){

}

Example2DScroll::GameMainScene::~GameMainScene() {

}

void Example2DScroll::GameMainScene::LoadData() {

	_manager.GetGame().GetTextureManager().Load("Player", "Assets/Textures/16Player.png");

}

void Example2DScroll::GameMainScene::LoadObjects() {

	Player& player = CreateGameObject<Player>();

	GameObject& child = CreateChildObject<GameObject>(&player);
	SpriteComponent& sprite = child.CreateComponent<SpriteComponent>(10);
	sprite.SetTexture("Rectangle");
	child.Scale(0.25f);
	child.Position(Vector2Int(0, 20));

	_camera = &CreateGameObject<Camera>();

	FollowCameraComponent& cameraComp = _camera->CreateComponent<FollowCameraComponent>();
	cameraComp.SetTargetObject(&player);
	_camera->SetCameraComponent(&cameraComp);

	ChangeSubScene<PlaySubScene>(player);
	_currentSubScene->Initialize();
}