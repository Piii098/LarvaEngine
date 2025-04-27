#include <algorithm>
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/UIScene.h"
#include "LarvaEngine/Core/MainScene.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

MainScene::MainScene(SceneManager& manager)
	: Scene(manager) {
}

MainScene::~MainScene() {
	Shutdown();
}

//==============================================================================
// パブリック関数
//==============================================================================


// ===== 初期設定 ===== //

/**
 * シーンの初期化
 *
 * データをロードし、基底クラスMainSceneで初期化する
 */
void MainScene::Initialize() {
	LoadData();
	LoadObjects();	// サブシーンのシャットダウン
}


// ===== ループ処理 ===== //

/**
 * 入力処理を行う
 *
 * アクティブ状態のサブシーン、UIシーンの入力処理を行う
 * 
 * 余地：UIシーンの一番上のみ入力処理を行う
 */
void MainScene::InputScene(const InputAction& input) {

	if (_state == STATE::INACTIVE || _state == STATE::PAUSE) return; // メインシーンが非アクティブ状態の場合は処理しない

	if (_currentSubScene != nullptr) {
			_currentSubScene->InputScene(input);
	}

	for (auto& ui : _uiScenes) {
		ui->ProcessInput(input);
	}
}


/**
 * 更新処理を行う
 *
 * アクティブ状態のサブシーン、UIシーンの更新処理を行う
 * クローズ状態のUIシーンを削除する
 */
void MainScene::UpdateScene(const float deltaTime) {
	
	if (_state == STATE::INACTIVE || _state == STATE::PAUSE) return;// メインシーンが非アクティブ状態の場合は処理しない
	
	// サブシーンの更新処理
	if (_currentSubScene != nullptr) {
		_currentSubScene->UpdateScene(deltaTime);
	}

	// UIシーンの更新処理
	for (auto& ui : _uiScenes) {
		if (ui->State() == Scene::STATE::ACTIVE) {
			ui->Update(deltaTime);
		}
	}

	// UIシーンの削除処理
	for (auto& ui : _uiScenes) {
		if (ui->State() == Scene::STATE::CLOSE) { // クローズ状態のUIシーンを削除
			ui.reset();
		}
	}

}

/**
 * 描写処理を行う
 *
 * アクティブ状態のサブシーン、UIシーンの物理更新処理を行う
 * Rendererクラスで呼び出され低解像度レンダリングの処理を行う
 * bufferLayerが一致するスプライトの描写処理を行う
 */
void MainScene::Render(Shader& shader, int bufferLayer) {

	if (_state == STATE::INACTIVE ) return; // メインシーンが非アクティブ状態の場合は処理しない
	
	for (auto& spri : _sprites) {

		// スプライトがアクティブかつ親オブジェクトがアクティブの場合描写
		if (spri->State() == Component::STATE::ACTIVE && spri->GetParent().State() == GameObject::STATE::ACTIVE) {
			if (spri->GetBufferLayer() == bufferLayer) { // 描写レイヤーが一致する場合のみ描写
				spri->Render(shader);
			}
		}

	}

}

/**
 * UIの描写処理を行う
 *
 * アクティブ状態のUIシーンの描写処理を行う
 * Rendererクラスで呼び出され実ウィンドウサイズでの描写処理を行う
 */
void MainScene::RenderUIs(Shader& shader) {

	if (_state == STATE::INACTIVE) return; // メインシーンが非アクティブ状態の場合は処理しない

	for (auto& ui : _uiScenes) {
		ui->Render(shader);
	}
}


// ===== シーン管理 ===== //


void MainScene::ReloadScene() {
	Scene::ReloadScene();

	for (auto& ui : _uiScenes) {
		ui->ReloadScene();
	}

	_uiScenes.clear();
	_currentSubScene.reset();

	LoadObjects();

}

/**
 * UIシーンを削除する
 *
 * 渡されたUIシーンを_uiScenesから削除する
 */
void MainScene::RemoveUIScene(UIScene* uiScene) {
	auto iter = std::find_if(_uiScenes.begin(), _uiScenes.end(), 
		[uiScene](const std::unique_ptr<UIScene>& ui) {
		return ui.get() == uiScene;
		});

	if (iter != _uiScenes.end()) {
		_uiScenes.erase(iter);
	}
}


// ===== データ管理 ===== //

/**
 * データを設定する
 *
 * キーと値をペアでデータを設定する
 */
void MainScene::SetData(const std::string& key, GameTypes::DataValue value) {
	_data[key] = value;
}

/**
 * データを更新する
 *
 * キーと値をペアでデータを更新する
 */
void MainScene::SetDataUpdate(const std::string& key, std::function<GameTypes::DataValue()> update) {
	_dataUpdate[key] = update;
}

/**
 * データを取得する
 *
 * キーを指定してデータを取得する
 * データが存在しない場合はstd::monostateを返す
 */
GameTypes::DataValue MainScene::GetData(const std::string& key) {
	// まずデータが存在するか確認
	auto iterData = _data.find(key);
	if (iterData == _data.end()) {
		return std::monostate{};  // データが存在しない
	}

	// ゲッターがあるか確認
	auto iterGetter = _dataUpdate.find(key);
	if (iterGetter != _dataUpdate.end()) {

		// ゲッターが存在する場合は現在値を取得して比較
		auto currentValue = iterGetter->second();
		auto& sceneValue = iterData->second;
		if (sceneValue != currentValue) {
			// 値が変わっていれば更新
			sceneValue = currentValue;
		}

	}

	// 最終的には_data内の値を返す
	return iterData->second;
}


//==============================================================================
// プロテクティッド関数
//==============================================================================

// ===== 終了処理 ===== //

/**
 * シャットダウン処理
 *
 * UnloadData()を呼び出す
 * 特別なシャットダウン処理が必要な場合はオーバーライドする
 */
void MainScene::Shutdown() {
	UnloadData();
	_uiScenes.clear();
	_currentSubScene.reset();
}
