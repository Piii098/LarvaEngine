#include <SDL3/SDL_timer.h>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include "Components/TextureComponent.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Game.h"
#include "Utilities/Frame.h"
#include "Utilities/Input.h"
#include "GameObjects/Camera.h"
#include "GameObjects/TileMapObject.h"
#include "Components/TileMapComponent.h"
#include "Renderer.h"
#include "PhysWorld2D.h"
#include "GameObjects/Background.h"
#include "AssetManagers/AssetManager.h"
#include "AssetManagers/TileMap.h"
#include "GameObjects/TileMapObject.h"

#pragma region Constructor:Destructor

Game::Game()
	: _renderer(nullptr)
	, _isRunning(true)
	, _isUpdating(false)
	, _brightness(1.f)
	, _camera(nullptr)
	, _player(nullptr)
	, _frame(nullptr)
	, _input(nullptr)
	, _physWorld(nullptr)
	, _textureManager(nullptr) 
	, _tileMapManager(nullptr) {
}

Game::~Game() {
	Shutdown();
}

#pragma endregion

#pragma region Public Functions

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		SDL_Log("Failed to intialized SDL : %s", SDL_GetError());
		return false;
	}

	_renderer = new Renderer(this);
	if (!_renderer->Initialize(1024.f, 768.f, 1024.f, 768.f)) {
		SDL_Log("Failed to intialized renderer");
		delete _renderer;
		_renderer = nullptr;
		return false;
	}

	_frame = new Frame();
	_input = new Input();
	_physWorld = new PhysWorld2D(this);
	_textureManager = new AssetManager<Texture>();
	_tileMapManager = new AssetManager<TileMap>();

	_frame->SetFixedDeltaTime(1.f / 60.f);

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
	UnloadData();
	delete _physWorld;
	_physWorld = nullptr;
	if (_renderer) {
		_renderer->Shutdown();
		delete _renderer;
		_renderer = nullptr;
	}
	SDL_Quit();
}

void Game::AddObject(GameObject* object) {
	if (_isUpdating) {
		_pendingObjects.emplace_back(object);
	} else {
		_objects.emplace_back(object);
	}
}

void Game::RemoveObject(GameObject* object) {

	// 親オブジェクトから削除
	if (object->GetParent()) {
		object->GetParent()->RemoveChildren(object);
	}

	// 子オブジェクトを削除
	for (auto child : object->GetChildren()) {
		delete child;
		child = nullptr;
	}

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

#pragma region Private Functions

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			_isRunning = false;
		}
	}

	const bool* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		_isRunning = false;
	}

	_input->Update();
	if (_input->IsInputDown(InputMap::INPUT_BRIGHT)) {
		_brightness += 0.1f;
	}
	if (_input->IsInputDown(InputMap::INPUT_BLEFT)) {
		_brightness -= 0.1f;
	}

	_isUpdating = true;
	for (auto& obj : _objects) {
		obj->ProcessInput(_input);
	}
	_isUpdating = false;
}

void Game::Update() {
	_frame->Update();
	while (_frame->ShouldDoFixedUpdate()) {
		float deltaTime = _frame->GetFixedDeltaTime();
		for (auto& obj : _objects) {
			obj->PhysUpdate(deltaTime);
		}
		_physWorld->Update(deltaTime);
		_frame->ConsumeFixedDeltaTime();
	}

	// レンダリング更新（補間処理を含む）
	_isUpdating = true;
	for (auto& obj : _objects) {
		obj->Update(_frame);
	}
	_isUpdating = false;

	for (auto pend : _pendingObjects) {
		pend->ComputeWorldTransform();
		_objects.emplace_back(pend);
	}
	_pendingObjects.clear();

	std::vector<GameObject*> deadObjects;
	for (auto obj : _objects) {
		if (obj->State() == GameObject::STATE::DEAD) {
			deadObjects.emplace_back(obj);
		}
	}

	for (auto obj : deadObjects) {
		delete obj;
	}
	_objects.erase(std::remove_if(_objects.begin(), _objects.end(),
		[](GameObject* obj) { return obj->State() == GameObject::STATE::DEAD; }), _objects.end());
}

void Game::ProcessOutput() {

	_renderer->SetLightPos(_player->Position());
	_renderer->Draw();
}

void Game::LoadData() {
	_textureManager->Load("Player", "Assets/AKAGE.png");
	_textureManager->Load("Sky", "Assets/Sky.png");
	_textureManager->Load("Plain", "Assets/Plain.png");
	_textureManager->Load("Mountain", "Assets/Mountain.png");
	_textureManager->Load("Tile", "Assets/Tile.png");
	_textureManager->Load("RedBox", "Assets/RedBox.png");
	_tileMapManager->Load("TileMap", "Assets/test.csv");
	_player = new Player(this);
	_tileMapObject = new TileMapObject(this);
	_tileMapObject->SetOnTile(_player, -2);
	_renderer->SetLightPos(_player->Position());
	_camera = new Camera(this);
	_camera->Position(_player->Position());
	new Background(this);
}

void Game::UnloadData() {
	while (!_objects.empty()) {
		delete _objects.back();
	}

	if (_renderer) {
		_renderer->UnloadData();
	}
}

#pragma endregion
