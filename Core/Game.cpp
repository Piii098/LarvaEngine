
#include <iostream>
#include <algorithm>
#include <SDL3_ttf/SDL_ttf.h>
#include "Scene/Scene.h"
#include "Scene/Game/TestScene.h"
#include "Components/Draw/TextureComponent.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Game.h"
#include "Utilities/Frame.h"
#include "Utilities/Input.h"
#include "GameObjects/Camera.h"
#include "GameObjects/TileMapObject.h"
#include "Components/TileMapComponent.h"
#include "Renderer/Renderer.h"
#include "Phys/PhysWorld2D.h"
#include "GameObjects/Background.h"
#include "AssetManagers/AssetManager.h"
#include "AssetManagers/AssetData/TileMap.h"
#include "GameObjects/TileMapObject.h"
#include "Scene/SceneManager.h"
#include "Audio/AudioSystem.h"

#pragma region Constructor:Destructor

Game::Game()
	: _renderer(nullptr)
	, _isRunning(true)
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

	if (!TTF_Init()) {
		SDL_Log("Failed to intialized TTF : %s", SDL_GetError());
		return false;
	}

	_renderer = new Renderer(this);
	if (!_renderer->Initialize(960.f, 540.f, 480.f, 270.f)) {
		SDL_Log("Failed to intialized renderer");
		delete _renderer;
		_renderer = nullptr;
		return false;
	}

	_audioSystem = new AudioSystem(this);
	if (!_audioSystem->Initialize()) {
		SDL_Log("Failed to intialized audio system");
		delete _audioSystem;
		_audioSystem = nullptr;
		return false;
	}

	_frame = new Frame();
	_input = new Input();
	_physWorld = new PhysWorld2D(this);
	_textureManager = new AssetManager<Texture>();
	_tileMapManager = new AssetManager<TileMap>();
	_fontManager = new AssetManager<Font>();
	_audioManager = new AssetManager<Audio>();

	_frame->SetFixedDeltaTime(1.f / 60.f);

	LoadScene();

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
	
	_sceneManager->Shutdown();
	delete _sceneManager;
	_sceneManager = nullptr;

	delete _physWorld;
	_physWorld = nullptr;
	if (_renderer) {
		_renderer->Shutdown();
		delete _renderer;
		_renderer = nullptr;
	}
	SDL_Quit();
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

	_input->Update();
	
	_sceneManager->ProcessInput(_input);

}

void Game::Update() {
	_frame->Update();

	PhysUpdate();

	UpdateScene();

	_audioSystem->Update(_frame->DeltaTime());
}

void Game::PhysUpdate() {

	while (_frame->ShouldDoFixedUpdate()) {
		float deltaTime = _frame->GetFixedDeltaTime();
		
		_sceneManager->PhysUpdate(deltaTime);

		_physWorld->Update(deltaTime);
		_frame->ConsumeFixedDeltaTime();
	}

}

void Game::UpdateScene() {
	_sceneManager->Update(_frame->DeltaTime());
}

void Game::ProcessOutput() {

	_renderer->Render();
}

void Game::LoadScene() {
	_sceneManager = new SceneManager(this);
	_sceneManager->Initialize();
	//TestScene* scene = _sceneManager->ChangeGameScene<TestScene>();
	//Player* player = new Player(scene);

}

void Game::UnloadData() {

	if (_renderer) {
		_renderer->UnloadData();
	}
}

#pragma endregion
