#include "LarvaEngine/Core/UIScene.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Core/Resources/Font.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"

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

void UIScene::Initialize() {
	LoadData();
}

void UIScene::InputScene(Input* input) {
	Scene::InputScene(input);
	InputUI(input);
}

void UIScene::UpdateScene(float deltaTime) {
	Scene::UpdateScene(deltaTime);
	UpdateUI(deltaTime);
}

void UIScene::Render(Shader* shader) {
	if (_state == STATE::ACTIVE) {
		for (auto& spri : _sprites) {
			if (spri->GetState() == Component::STATE::ACTIVE && spri->GetParent()->State() == GameObject::STATE::ACTIVE) {
				
				spri->Render(shader);
		
			}
		}
	}
}

#pragma endregion

#pragma region プライベート関数

#pragma endregion