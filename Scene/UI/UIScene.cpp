#include "Scene/UI/UIScene.h"
#include "Scene/Scene.h"
#include "Renderer/Shader.h"
#include "AssetManagers/AssetData/Texture.h"
#include "Utilities/Font.h"
#include "Scene/SceneManager.h"
#include "Core/Game.h"
#include "AssetManagers/AssetManager.h"

#pragma region コンストラクタ:デストラクタ

UIScene::UIScene(MainScene* scene)
	: Scene(scene->GetManager()){
	_scene = scene;
	_scene->AddUIScene(this);
}

UIScene::~UIScene() {
	_scene->RemoveUIScene(this);
}

#pragma endregion

#pragma region パブリック関数

void UIScene::InputScene(Input* input) {
	Scene::InputScene(input);
	InputUI(input);
}

void UIScene::UpdateScene(float deltaTime) {
	Scene::UpdateScene(deltaTime);
	UpdateUI(deltaTime);
}

void UIScene::Render(Shader* shader) {
	for (auto& obj : _objects) {
		obj->RenderUI(shader);
	}
}

#pragma endregion

#pragma region プライベート関数

#pragma endregion