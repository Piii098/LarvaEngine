#include "LarvaEngine/Core/SceneManager.h" 
#include "LarvaEngine/Core/Game.h" 
#include "LarvaEngine/Core/Scene.h" 
#include "LarvaEngine/Core/Resources/AssetManager.h"

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

/**
 * 入力処理を行う
 * 
 * 現在のメインシーンの入力処理を行う
 */
void SceneManager::ProcessInput(const InputAction& input) {
	_currentMainScene->ProcessInput(input);
}

/**
 * 更新処理を行う
 *
 * 現在のメインシーンの更新処理を行う
 */
void SceneManager::Update(const float deltaTime) {
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


//==============================================================================
// プライベート関数
//==============================================================================

/**
 * シーンの破壊
 *	
 * 現在のメインシーンを破棄する
 */
void SceneManager::DestroyScene() {
	
}

/**
 * データのロード
 *
 * ゲームクラスのアセットマネージャーを使用してデータをロードする
 * ゲーム全体で使用するデータをロードする
 */
void SceneManager::LoadData() {

	_game.GetFontManager().Load("DelaSuko", "Assets/Fonts/MOBO-Bold.otf");

}


