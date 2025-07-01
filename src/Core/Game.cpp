#include <SDL3_ttf/SDL_ttf.h>
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/FrameSystem.h"
#include "LarvaEngine/Renderer/Renderer.h"
#include "LarvaEngine/Renderer/2D/Renderer2D.h"
#include "LarvaEngine/Renderer/3D/Renderer3D.h"
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/Physics/PhysWorld3D.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Core/Resources/TileMap.h"
#include "LarvaEngine/Core/Resources/Font.h"
#include "LarvaEngine/Renderer/3D/Mesh.h"
#include "LarvaEngine/Core/Resources/Model.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Audio/AudioSystem.h"
#include "LarvaEngine/Input/InputAction.h"
#include "LarvaEngine/Input/InputSystem.h"

/// ====================================================================================================
/// コンストラクタ・デストラクタ
/// ====================================================================================================

Game::Game()
	: _renderer(nullptr)
	, _isRunning(true)
	, _frameSystem(nullptr)
	, _physWorld2D(nullptr)
	, _physWorld3D(nullptr)
	, _textureManager(nullptr) 
	, _tileMapManager(nullptr)
	, _windowHeight(0)
	, _windowWidth(0)
	, _lowResHeight(0)
	, _lowResWidth(0)
	, _useMultithreading(false)
	, _rendererMode(RENDERER_MODE::MODE_2D){
}

Game::~Game() {
	
}

/// ====================================================================================================
/// パブリック関数
/// ====================================================================================================

/**
 * ウィンドウを設定する
 * 
 * 受け取ったウィンドウの幅と高さ、低解像度レンダリングの幅と高さを設定する
 */
void Game::SetWindowsSize(int windowWidth, int windowHeight, int lowResWidth, int lowResHeight) {
	// ウィンドウサイズを設定
	_windowHeight = windowHeight;
	_windowWidth = windowWidth;
	// 低解像度レンダリングのサイズを設定
	_lowResHeight = lowResHeight == -1 ? windowHeight : lowResHeight; // 低解像度レンダリングのサイズを設定
	_lowResWidth = lowResWidth == -1 ? windowWidth : lowResWidth; // 低解像度レンダリングのサイズを設定
}

/**
 * ウィンドウ名を設定する
 *
 * 受け取ったウィンドウ名を設定する
 */

void Game::SetWindowsName(const std::string& name) {
	// ウィンドウ名を設定
	_windowName = name;
}

/**
 * ゲームの初期化
 * 
 * SDLの初期化、ウィンドウの初期化、TTFの初期化、
 * システムコンポーネントの初期化、アセットマネージャーの初期化を行う
 */
bool Game::Initialize() {
	
	// SDLの初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) == 0) {
		SDL_Log("Failed to intialized SDL : %s", SDL_GetError());
		return false;
	}
	// TTFの初期化
	if (!TTF_Init()) {
		SDL_Log("Failed to intialized TTF : %s", SDL_GetError());
		return false;
	}

	// ウィンドウ、レンダラーの初期化
	switch(_rendererMode) {
	case RENDERER_MODE::MODE_2D:
		_renderer = std::make_unique<Renderer2D>(*this);
		_physWorld2D = std::make_unique<PhysWorld2D>(*this); // 物理システムの初期化
		break;
	case RENDERER_MODE::MODE_3D:
		_renderer = std::make_unique<Renderer3D>(*this);
		_physWorld3D = std::make_unique<PhysWorld3D>(*this); // 物理システムの初期化
		break;
	default:
		SDL_Log("Invalid renderer mode");
		return false;
		break;
	}

	_renderer->SetLowResSize(_lowResWidth, _lowResHeight); // 低解像度レンダリングのサイズを設定
	_renderer->SetWindowSize(_windowWidth, _windowHeight); // ウィンドウのサイズを設定

	if (!_renderer->Initialize(_windowName)) {
		SDL_Log("Failed to intialized renderer");
		return false;
	}

	// オーディオシステムの初期化
	_audioSystem = std::make_unique<AudioSystem>(*this);

	if (!_audioSystem->Initialize()) {
		SDL_Log("Failed to intialized audio system");
		return false;
	}

	// ゲームパッドの初期化
	_inputSystem = std::make_unique<InputSystem>(*this);
	if (!_inputSystem->Initialize()) {
		SDL_Log("Failed to intialized input system");
		return false;
	}

	_inputAction = std::make_unique<InputAction>(*_inputSystem.get()); // 入力アクションの初期化
	_frameSystem = std::make_unique<FrameSystem>();					  // フレームシステムの初期化	
	_textureManager = std::make_unique<AssetManager<Texture>>(*this);      // テクスチャマネージャの初期化
	_tileMapManager = std::make_unique<AssetManager<TileMap>>(*this);	  // タイルマップマネージャの初期化
	_fontManager = std::make_unique<AssetManager<Font>>(*this);		      // フォントマネージャの初期化
	_meshManager = std::make_unique<AssetManager<Mesh>>(*this);	  // メッシュマネージャの初期化
	_modelManager = std::make_unique<AssetManager<Model>>(*this);	  // モデルマネージャの初期化

	GetTextureManager().Load("ToonRampTexture", "Assets/Shaders/toon.png");
	GetTextureManager().Load("Sample", "Assets/Textures/bg_1.png");

	// フレームレートの設定
	SetAction();
	LoadScene();

	return true;
}

/**
 * ゲームの実行
 *
 * ゲームのメインループを実行する
 */
void Game::RunLoop() {
	while (_isRunning) {
		ProcessInput();
		Update();
		ProcessOutput();
	}
}

/**
 * ゲームの終了
 *
 * ゲームの終了処理を行う
 */
void Game::Shutdown() {

	UnloadData();
	
	SDL_Quit();
}

/**
 * 入力処理を行う
 * 
 * イベントのポーリングを行い、入力システムに処理を委譲する
 * また、シーンマネージャ->シーン->ゲームオブジェクトの順に入力処理を行う
 */
void Game::ProcessInput() {

	// 入力システムの事前更新
	_inputSystem->PreUpdate();

	// イベントのポーリング
	SDL_Event event;
	while (SDL_PollEvent(&event)) {  // イベントキューにイベントがある限り処理を続ける
		switch (event.type) {        // event.typeでイベントの種類を判別
		case SDL_EVENT_QUIT:
			_isRunning = false;
			break;
		case SDL_EVENT_MOUSE_WHEEL:     // マウスホイールイベント
		case SDL_EVENT_GAMEPAD_ADDED:   // ゲームパッド接続イベント
		case SDL_EVENT_GAMEPAD_REMOVED: // ゲームパッド切断イベント
			_inputSystem->ProcessInput(event);
			break;
		default:
			break;
		}
	}

	// 入力システムの更新
	_inputSystem->Update(_frameSystem->DeltaTime());
	
	// シーンの入力処理
	_sceneManager->ProcessInput(*_inputAction.get());

}

/// ====================================================================================================
/// プライベート関数
/// ====================================================================================================

/**
 * ゲームの更新処理
 *
 * フレームシステムの更新、物理更新、シーンの更新、オーディオシステムの更新を行う
 */
void Game::Update() {

	// フレームシステムの更新
	_frameSystem->Update();

	while (_frameSystem->ShouldRunFixedTimeStep())
	{
		FixedUpdate();
		LateUpdate();
	}

	// シーンの更新
	UpdateScene();

	// オーディオシステムの更新
	_audioSystem->Update(_frameSystem->DeltaTime());

	//SDL_Log("FPS : %f", _frameSystem->Fps());
}

/**
 * 物理更新処理
 *
 * 固定フレームレートで物理更新を行う
 * また、シーンマネージャ->シーン->ゲームオブジェクトの順に物理更新を行う
 * 物理システムの更新を行う
 */
void Game::FixedUpdate() {

	_sceneManager->FixedUpdate(_frameSystem->FixedDeltaTime());

	if (_rendererMode == RENDERER_MODE::MODE_2D){
		_physWorld2D->FixedUpdate(_frameSystem->FixedDeltaTime());
	}
	else if (_rendererMode == RENDERER_MODE::MODE_3D)
	{
		_physWorld3D->FixedUpdate(_frameSystem->FixedDeltaTime());
	}
}

void Game::LateUpdate() {
	_sceneManager->LateUpdate(_frameSystem->FixedDeltaTime());
}

/**
 * シーンの更新処理
 *
 * シーンマネージャ->シーン->ゲームオブジェクトの順に更新処理を行う
 */
void Game::UpdateScene() {
	_sceneManager->Update(_frameSystem->DeltaTime());
}

void Game::ProcessOutput() {

	// SDL_Log("FPS : %f", _frameSystem->FPS());
	// SDL_Log("GameTime : %f", _frameSystem->GameTime());
	// SDL_Log("DeltaTime : %f", _frameSystem->DeltaTime());

	_renderer->Render();
}

/**
 * シーンを読み込む
 * 
 * 事前に初期シーンの設定があれば、そのシーンを読み込む
 * 無ければ、MainSceneを読み込む
 */
void Game::LoadScene() {
	_sceneManager = std::make_unique<SceneManager>(*this);

	// カスタム初期化関数があれば使用
	if (_initialSceneSetup) {
		_initialSceneSetup(*_sceneManager.get());
	}
	else {
		// デフォルト初期化（従来の挙動）
		_sceneManager->Initialize();
	}
}

void Game::SetAction(){

	_inputAction->MapAction("Left", SDL_SCANCODE_A);
	_inputAction->MapAction("Right", SDL_SCANCODE_D);
	_inputAction->MapAction("Up", SDL_SCANCODE_W);
	_inputAction->MapAction("Down", SDL_SCANCODE_S);
	_inputAction->MapAction("Select", SDL_SCANCODE_SPACE);
	_inputAction->MapAction("Jump", SDL_SCANCODE_SPACE);
	_inputAction->MapAction("Menu", SDL_SCANCODE_TAB);

	_inputAction->MapAction("Left", SDL_GAMEPAD_BUTTON_DPAD_LEFT);
	_inputAction->MapAction("Right", SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
	_inputAction->MapAction("Up", SDL_GAMEPAD_BUTTON_DPAD_UP);
	_inputAction->MapAction("Down", SDL_GAMEPAD_BUTTON_DPAD_DOWN);
	_inputAction->MapAction("Select", SDL_GAMEPAD_BUTTON_SOUTH);
	_inputAction->MapAction("Jump", SDL_GAMEPAD_BUTTON_SOUTH);
	_inputAction->MapAction("Menu", SDL_GAMEPAD_BUTTON_BACK);

	_inputAction->MapActionAxis("Horizontal", SDL_SCANCODE_A, SDL_SCANCODE_D);
	_inputAction->MapActionAxis("Vertical", SDL_SCANCODE_S, SDL_SCANCODE_W);
	
	_inputAction->MapActionAxis("Horizontal", SDL_GAMEPAD_AXIS_LEFTX);
	_inputAction->MapActionAxis("Vertical", SDL_GAMEPAD_AXIS_LEFTY);
}

void Game::UnloadData() {

	if (_renderer) {
		//_renderer->UnloadData();
	}
}
