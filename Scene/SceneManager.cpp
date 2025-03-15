#include "Scene/SceneManager.h" 
#include "Core/Game.h" 
#include "Scene/Game/TestScene.h"
#include "Scene/Game/Title/TitleScene.h"
#include "Scene/Scene.h" 
#include "Utilities/Input.h"
#include "AssetManagers/AssetManager.h"

#pragma region コンストラクタ:デストラクタ

SceneManager::SceneManager(Game* game)
    : _game(game) {
}

SceneManager::~SceneManager() {

}

#pragma endregion

#pragma region パブリック関数

void SceneManager::Initialize() {
	LoadData();

	_currentMainScene = new TitleScene(this);
	_currentMainScene->Initialize();
}

void SceneManager::ProcessInput(Input* input) {
	_currentMainScene->ProcessInput(input);
}

void SceneManager::Update(float deltaTime) {
	_currentMainScene->Update(deltaTime);
}

void SceneManager::PhysUpdate(float deltaTime) {
	_currentMainScene->PhysUpdate(deltaTime);
}

void SceneManager::Output() {
	_currentMainScene->Output();
}   

void SceneManager::Shutdown() {
	//DestroyScene();
}

void SceneManager::DestroyScene() {
	
	if (_currentMainScene != nullptr) {
		_currentMainScene->Shutdown();
		delete _currentMainScene;
		_currentMainScene = nullptr;
	}
}

#pragma endregion

#pragma region プライベート関数

void SceneManager::LoadData() {
	_game->GetFontManager()->Load("DelaSuko", "Assets/DelaSukoGothicOne-R.ttf");

}


#pragma endregion
