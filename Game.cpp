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

#pragma region �p�u���b�N�֐�

bool Game::Initialize() {

	/*�V�X�e��������*/
	
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (!sdlResult) {
		SDL_Log("Failed to initialize a system : %s", SDL_GetError());
		return false;
	}

	/*�V�F�[�_�[������*/

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
	
	/*�Q�[��������*/
	
		
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

#pragma region �v���C�x�[�g�֐�

void Game::ProcessInput() {

	/*�C�x���g�쓮*/
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_EVENT_QUIT:
			_isRunning = false;
			break;
		}

	}

	/*�L�[�{�[�h�C�x���g�쓮*/
	const bool* state = SDL_GetKeyboardState(NULL); //�S�L�[�`�F�b�N


	if (state[SDL_SCANCODE_ESCAPE]) {
		_isRunning = false;
	}

	/*�Q�[�����e*/

	_input->Update(); // �C���v�b�g
	
	
	_isUpdating = true;
	for (auto& obj : _objects) {
		obj->ProcessInput(_input);
	}
	_isUpdating = false;

}

void Game::Update() {

	/*�V�X�e���X�V*/
	_frame->Update(); // �t���[��

	/*�Q�[�����e*/

	_isUpdating = true;
	for (auto& obj : _objects) { // �X�V����
		obj->Update(_frame);
	}
	_isUpdating = false;

	_physWorld->Update();

	for (auto pend : _pendingObjects) { // �ҋ@�I�u�W�F�N�g���X�V�I�u�W�F�N�g�ɒǉ�
		pend->ComputeWorldTransform();
		_objects.emplace_back(pend);
	}
	_pendingObjects.clear();

	std::vector<GameObject*> deadObjects; // DEAD��Ԃ̃I�u�W�F�N�g���폜
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

	/*�f�[�^���[�h*/

	SpriteComponent::S_SpriteManager().Load("Assets/AKAGE.png");
	SpriteComponent::S_SpriteManager().Load("Assets/RedBox.png");
	SpriteComponent::S_SpriteManager().Load("Assets/Tile.png");
	TileMapComponent::S_TileMapManager().Load("Assets/Test.csv");

	/*�Q�[���I�u�W�F�N�g*/

	//Player* play = new Player(this);
	TileMap* tileMap = new TileMap(this);
	_player = new Player(this);
	//TestObject* tesObj = new TestObject(this);
	_player->PlacePlayerAtSpawn(tileMap->GetTileMapComponent());
	_camera = new Camera(this);
}

#pragma endregion
