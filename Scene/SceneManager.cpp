#include "Scene/SceneManager.h" 
#include "Core/Game.h" 
#include "Scene/Scene.h" 
#include "Utilities/Input.h"

#pragma region コンストラクタ:デストラクタ

SceneManager::SceneManager(Game* game)
    : _game(game) {
}

SceneManager::~SceneManager() {

}

#pragma endregion

#pragma region パブリック関数

void SceneManager::Initialize() {
	_currentScene = new Scene(this);
	_currentScene->Initialize();
}

void SceneManager::ProcessInput(Input* input) {
	_currentScene->ProcessInput(input);
}

void SceneManager::Update(float deltaTime) {
	_currentScene->Update(deltaTime);
}

void SceneManager::PhysUpdate(float deltaTime) {
	_currentScene->PhysUpdate(deltaTime);
}

void SceneManager::Output() {
	_currentScene->Output();
}   

void SceneManager::Shutdown() {
	DestroyScene();
}

void SceneManager::DestroyScene() {
	_currentScene->Shutdown();
	if (_currentScene != nullptr) {
		_currentScene->Shutdown();
		delete _currentScene;
		_currentScene = nullptr;
	}
}

#pragma endregion