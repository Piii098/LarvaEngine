#include "Examples/Game/2DScroll/TitleMainScene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "Examples/Game/2DScroll/TitleUIScene.h"

Example2DScroll::TitleMainScene::TitleMainScene(SceneManager& manager)
	: MainScene(manager) {
}

Example2DScroll::TitleMainScene::~TitleMainScene() {
}

void Example2DScroll::TitleMainScene::LoadData() {
	_manager.GetGame().GetTextureManager().Load("Button", "Assets/Textures/Button.png");
}

void Example2DScroll::TitleMainScene::LoadObjects() {
	CreateUIScene<TitleUIScene>();
}