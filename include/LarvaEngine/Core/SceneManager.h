#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "LarvaEngine/Core/MainScene.h"

// ===== 前方宣言 =====
class UIScene;
class Game;

/// @brief シーン管理クラス
///
/// 単一のメインシーンを管理する
/// シーンの切り替え、初期化、更新、描画、終了処理を行う
class SceneManager {
public:

	SceneManager(Game& game);
	~SceneManager();


	// ===== 初期設定 ===== //

	/// @brief シーンの初期化
	/// 基底クラスMainSceneで初期化する
	void Initialize();

	/// @brief シーンの初期化
	/// 指定したシーンクラスで初期化する
	/// @tparam T 初期のシーンクラス(MainSceneを継承したクラス)
	/// @param args シーンの初期化に必要な引数( 第一引数のシーンマネージャーの参照は自動で設定される )
	template <typename T, typename... Args>
	void InitializeWithScene(Args&&... args) {
		LoadData();

		std::unique_ptr<T> tempScene = std::make_unique<T>(*this, std::forward<Args>(args)...);
		_currentMainScene = std::move(tempScene);
		_currentMainScene->Initialize();
	}


	// ====== ループ処理 ====== //

	/// @brief メインシーンの入力処理
	/// 現在のメインシーンの入力処理を行う
	/// @param input 入力情報
	void ProcessInput(const InputAction& input);

	/// @brief メインシーンの更新処理
	/// 現在のメインシーンの更新処理を行う
	/// @param deltaTime フレーム間の時間
	void Update(float deltaTime);

	/// @brief メインシーンの物理更新処理
	/// 現在のメインシーンの物理更新処理を行う
	/// @param deltaTime 補正されたフレーム間の時間
	void FixedUpdate(float deltaTime);

	void LateUpdate(float deltaTime);

	/// @brief メインシーンの出力処理
	/// 現在のメインシーンの出力処理行う
	void Output();


	// ====== シーン関連 ====== //

	void RelaodScene();

	// SceneManager.h の改良版
	template <typename T, typename... Args>
	void RequestSceneChange(Args&& ...args) {
		_pendingSceneChange = [this, args...]() {
			this->ChangeScene<T>(std::forward<Args>(args)...);
			};
	}

	/// @brief シーンの切り替え
	/// 現在のメインシーンを破棄し、新しいシーンを生成する
	/// @tparam T 切り替えるシーンクラス(MainSceneを継承したクラス)
	/// @param args シーンの初期化に必要な引数( 第一引数のシーンマネージャーの参照は自動で設定される )
	template <typename T, typename... Args>
	T* ChangeScene(Args&& ...args) {
		// 新しいシーンを作成
		std::unique_ptr<T> newScene = std::make_unique<T>(*this, std::forward<Args>(args)...);

		// 初期化
		newScene->Initialize();

		// 古いシーンを破棄して新しいシーンを設定
		_currentMainScene.reset();
		_currentMainScene = std::move(newScene);

		return static_cast<T*>(_currentMainScene.get());
	
	}

	/// @brief シーンの破棄
	/// 現在のメインシーンを破棄する
	void DestroyScene();

	/// @brief シーンのリロード
	/// 現在のメインシーンを破棄し、再度初期化する
	void ReloadMainScene();


	// ===== ゲッター ===== //

	Game& GetGame() { return _game; }
	MainScene& GetCurrentMainScene() { return *_currentMainScene.get(); }
	std::vector<std::unique_ptr<UIScene>>& GetCurrentUIScenes() { return _currentMainScene->GetUIScenes(); }
	const Game& GetGame() const { return _game; }
	const MainScene& GetCurrentMainScene() const { return *_currentMainScene.get(); }
	const std::vector<std::unique_ptr<UIScene>>& GetCurrentUIScenes() const { return _currentMainScene->GetUIScenes(); }

private:

	// ===== リソース管理 ===== //

	/// @brief データの読み込み
	/// ゲームの初期化時にデータを読み込む
	/// ゲーム全体で使用するリソースを読み込む(フォントなど)
	void LoadData();


	// ===== シャットダウン処理 ===== //

	/// @brief シャットダウン処理
	/// シーンの破棄処理を行う
	/// リソースを解放する	
	void Shutdown();

	std::function<void()> _pendingSceneChange;	///< シーン変更のリクエスト


	Game& _game;									///< ゲームクラス
	std::unique_ptr<MainScene> _currentMainScene;	///< 現在のメインシーン
	bool _isUpdating;								///< 更新中かどうか

};