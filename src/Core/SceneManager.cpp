﻿#include "LarvaEngine/Core/SceneManager.h" 
#include "LarvaEngine/Core/Game.h" 
#include "LarvaEngine/Core/Scene.h" 
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Input/InputAction.h"

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

SceneManager::SceneManager(Game& game)
    : _game(game) {
}

SceneManager::~SceneManager() {
	Shutdown();
}


//==============================================================================
// パブリック関数
//==============================================================================

/**
 * シーンの初期化
 * 
 * データをロードし、基底クラスMainSceneで初期化する
 */
void SceneManager::Initialize() {
	LoadData();

	_currentMainScene = std::make_unique<MainScene>(*this);

	_currentMainScene->Initialize();
}


// ===== ループ処理 ===== //

/**
 * 入力処理を行う
 * 
 * 現在のメインシーンの入力処理を行う
 */
void SceneManager::ProcessInput(const InputAction& input) {
	if (input.IsKeyDown(SDL_SCANCODE_F1)) {
		_currentMainScene->ReloadScene();
	}
	_currentMainScene->ProcessInput(input);
}

/**
 * 更新処理を行う
 *
 * 現在のメインシーンの更新処理を行う
 */
void SceneManager::Update(const float deltaTime) {
	if (_pendingSceneChange) {
		auto sceneChange = _pendingSceneChange;
		_pendingSceneChange = nullptr;
		sceneChange();
	}

	_currentMainScene->Update(deltaTime);
}

/**
 * 物理更新処理を行う
 *
 * 現在のメインシーンの物理更新処理を行う
 */
void SceneManager::FixedUpdate(const float deltaTime) {
	_currentMainScene->FixedUpdate(deltaTime);
}

void SceneManager::LateUpdate(float deltTime) {
	_currentMainScene->LateUpdate(deltTime);
}

/**
 * 出力処理を行う
 *
 * 現在のメインシーンの出力処理を行う
 */
void SceneManager::Output() {
	//_currentMainScene->Output();
}   

/**
 * シーンの切り替え
 *
 * 現在のメインシーンを破棄し、新しいシーンを生成する
 */
void SceneManager::Shutdown() {
	
}


// ===== シーン関連 ===== //



/**
 * シーンの破壊
 *
 * 現在のメインシーンを破棄する
 */
void SceneManager::DestroyScene() {

}

/**
 * シーンのリロード
 *
 * 現在のメインシーンを破棄し、再度初期化する
 */
void SceneManager::ReloadMainScene() {
	if (_currentMainScene) {
		// 現在のシーンを破棄
		_currentMainScene.reset();

		// 新しいシーンを生成
		_currentMainScene = std::make_unique<MainScene>(*this);
		_currentMainScene->Initialize();
	}
}

//==============================================================================
// プライベート関数
//==============================================================================

/**
 * データのロード
 *
 * ゲームクラスのアセットマネージャーを使用してデータをロードする
 * ゲーム全体で使用するデータをロードする
 */
void SceneManager::LoadData() {

	_game.GetFontManager().Load("MOBO", "Assets/Fonts/MOBO-Bold.otf");

	_game.GetTextureManager().Load("Rectangle", "Assets/Textures/Rectangle.png");

}


