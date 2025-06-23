#include "Examples/2D/Scroll/TestMainScene.h"
#include "Examples/2D/Scroll/MenuUIScene.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"

Example2DScroll::TestMainScene::TestMainScene(SceneManager& manager)
	: MainScene(manager) {
}

Example2DScroll::TestMainScene::~TestMainScene() {
}

void Example2DScroll::TestMainScene::LoadData() {
	_manager.GetGame().GetTextureManager().Load("Button", "Assets/Textures/Button.png");
}

void Example2DScroll::TestMainScene::LoadObjects() {
	_camera = &CreateGameObject<Camera>();
	CreateUIScene<MenuUIScene>();
}