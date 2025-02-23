#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include "Components/SpriteComponent.h"
#include "Components/RigidbodyComponent.h"
#include "Components/MoveInputComponent.h"
#include "Components/TileMapSpriteComponent.h"
#include "Polygon/VertexArray.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Game.h"
#include "Utilities/Frame.h"
#include "Utilities/Input.h"
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
#include "Singletons/ShaderManager.h"
#include "GameObjects/Camera.h"
#include "GameObjects/TileMap.h"
#include "Components/TileMapComponent.h"
#include "GameObjects/Player.h"
#include "Renderer.h"
#include "PhysWorld2D.h"
#include "GameObjects/TestObject.h"

#pragma region Constructor:Destructer

Game::Game()
	: _renderer(nullptr)
	, _isRunning(true)
	, _isUpdating(false){

}

Game::~Game() {

}

#pragma endregion

#pragma region パブリック関数

bool Game::Initialize() {

	/*システム初期化*/
	
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (!sdlResult) {
		SDL_Log("Failed to initialize a system : %s", SDL_GetError());
		return false;
	}

	/*シェーダー初期化*/

	_renderer = new Renderer(this);
	if (!_renderer->Initialize(1280.f, 720.f)) {
		SDL_Log("Failed to initialized renderer");
		delete _renderer;
		_renderer = nullptr;
		return false;
	}

	_frame = new Frame();
	_input = new Input();
	_physWorld = new PhysWorld2D(this);
	
	/*ゲーム初期化*/
	
		
	LoadData();

	return true;

}

void Game::RunLoop() {

	while (_isRunning) {
		ProcessInput();
		Update();
		ProcessOutput();
	}

}

void Game::Shutdown() {
	SDL_Quit();

}

void Game::AddObject(GameObject* object) {
	if (_isUpdating) {
		_pendingObjects.emplace_back(object);
	}
	else {
		_objects.emplace_back(object);
	}
}

void Game::RemoveObject(GameObject* object) {
	
	auto iter = std::find(_pendingObjects.begin(), _pendingObjects.end(), object);
	if (iter != _pendingObjects.end()) {
		std::iter_swap(iter, _pendingObjects.end() - 1);
		_pendingObjects.pop_back();
	}

	iter = std::find(_objects.begin(), _objects.end(), object);
	if (iter != _objects.end()) {
		std::iter_swap(iter, _objects.end() - 1);
		_objects.pop_back();
	}
}

#pragma endregion

#pragma region プライベート関数

void Game::ProcessInput() {

	/*イベント駆動*/
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_EVENT_QUIT:
			_isRunning = false;
			break;
		}

	}

	/*キーボードイベント駆動*/
	const bool* state = SDL_GetKeyboardState(NULL); //全キーチェック


	if (state[SDL_SCANCODE_ESCAPE]) {
		_isRunning = false;
	}

	/*ゲーム内容*/

	_input->Update(); // インプット
	
	
	_isUpdating = true;
	for (auto& obj : _objects) {
		obj->ProcessInput(_input);
	}
	_isUpdating = false;

}

void Game::Update() {

	/*システム更新*/
	_frame->Update(); // フレーム

	/*ゲーム内容*/

	_isUpdating = true;
	for (auto& obj : _objects) { // 更新処理
		obj->Update(_frame);
	}
	_isUpdating = false;

	_physWorld->Update();

	for (auto pend : _pendingObjects) { // 待機オブジェクトを更新オブジェクトに追加
		pend->ComputeWorldTransform();
		_objects.emplace_back(pend);
	}
	_pendingObjects.clear();

	std::vector<GameObject*> deadObjects; // DEAD状態のオブジェクトを削除
	for (auto obj : _objects) {
		if (obj->State() == GameObject::STATE::DEAD) {
			deadObjects.emplace_back(obj);
		}
	}

	for (auto obj : deadObjects) {
		delete obj;
	}

}

void Game::ProcessOutput() {

	_renderer->Draw();

}


void Game::LoadData() {

	/*データロード*/

	SpriteComponent::S_SpriteManager().Load("Assets/AKAGE.png");
	SpriteComponent::S_SpriteManager().Load("Assets/RedBox.png");
	SpriteComponent::S_SpriteManager().Load("Assets/Tile.png");
	TileMapComponent::S_TileMapManager().Load("Assets/Test.csv");

	/*ゲームオブジェクト*/

	//Player* play = new Player(this);
	TileMap* tileMap = new TileMap(this);
	_player = new Player(this);
	//TestObject* tesObj = new TestObject(this);
	_player->PlacePlayerAtSpawn(tileMap->GetTileMapComponent());
	_camera = new Camera(this);
}

#pragma endregion
