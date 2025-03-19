#include "LarvaEngine/Core/SceneManager.h" 
#include "LarvaEngine/Core/Game.h" 
#include "LarvaEngine/Examples/Game/Test/TestScene.h"
#include "LarvaEngine/Examples/Game/Title/TitleScene.h"
#include "LarvaEngine/Core/Scene.h" 
#include "LarvaEngine/Core/Events/Input.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"

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

	_currentMainScene = new MainScene(this);
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

AssetManager<Texture>* SceneManager::GetTextureManager() {
	return _game->GetTextureManager();
}

AssetManager<TileMap>* SceneManager::GetTileMapManager() {
	return _game->GetTileMapManager();
}

AssetManager<Font>* SceneManager::GetFontManager() {
	return _game->GetFontManager();
}

AssetManager<Audio>* SceneManager::GetAudioManager() {
	return _game->GetAudioManager();
}

#pragma endregion

#pragma region プライベート関数

void SceneManager::LoadData() {

	_game->GetFontManager()->Load("DelaSuko", "Assets/Fonts/MOBO-Bold.otf");

}


#pragma endregion
