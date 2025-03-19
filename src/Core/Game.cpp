
#include <iostream>
#include <algorithm>
#include <SDL3_ttf/SDL_ttf.h>
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Examples/Game/Test/TestScene.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/GameObjects/Player.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Frame.h"
#include "LarvaEngine/Core/Events/Input.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/GameObjects/TileMapObject.h"
#include "LarvaEngine/Components/TileMapComponent.h"
#include "LarvaEngine/Renderer/Renderer.h"
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/GameObjects/Background.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/TileMap.h"
#include "LarvaEngine/GameObjects/TileMapObject.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Audio/AudioSystem.h"

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
	, _tileMapManager(nullptr)
	, _windowHeight(0)
	, _windowWidth(0)
	, _lowResHeight(0)
	, _lowResWidth(0) {
}

Game::~Game() {
	Shutdown();
}

#pragma endregion

#pragma region Public Functions

void Game::SetWindowsSize(int windowWidth, int windowHeight, int lowResWidth, int lowResHeight) {
	_windowHeight = windowHeight;
	_windowWidth = windowWidth;
	_lowResHeight = lowResHeight;
	_lowResWidth = lowResWidth;
}

void Game::SetWindowsName(const std::string& name) {
	_windowName = name;
}

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

	int wWidth = _windowWidth ? _windowWidth : 960;
	int wHeigth = _windowHeight ? _windowHeight : 540;
	int lWidth = _lowResWidth ? _lowResWidth : 480;
	int lHeigth = _lowResHeight ? _lowResHeight : 270;
	std::string name = _windowName.empty() ? _windowName : "Game";

	if (!_renderer->Initialize(wWidth, wHeigth, lWidth, lHeigth, _windowName)) {
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

	// カスタム初期化関数があれば使用
	if (_initialSceneSetup) {
		_initialSceneSetup(_sceneManager);
	}
	else {
		// デフォルト初期化（従来の挙動）
		_sceneManager->Initialize();
	}
}

void Game::UnloadData() {

	if (_renderer) {
		_renderer->UnloadData();
	}
}

#pragma endregion
