#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <memory>
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SubScene.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Utilities/DataTypes.h"

// ===== 前方宣言 =====
class SceneManager;
class SpriteComponent;
class UIScreen;
class UIScene;

/// @brief メインシーンクラス
///
/// ゲームのメインシーンを管理する
/// サブシーン、UIシーンを管理する
/// カメラを持つ
/// リソースのロード、ゲームオブジェクトの生成、破棄を行う
/// 非アクティブ下では入力、更新、描写処理を行わない
class MainScene : public Scene {
public:

	// ===== コンストラクタ・デストラクタ ===== //

    MainScene(SceneManager& manager);

    virtual ~MainScene() override;

	// ===== 初期設定 ===== //
    
	/// @brief シーンの初期化
	/// LoadData()を呼び出す
	/// 特別な初期化処理が必要な場合はオーバーライドする
    virtual void Initialize() override;

	// ===== ループ処理 ===== //
	
    /// @brief 入力処理
	/// アクティブ状態のサブシーン、UIシーンの入力処理を行う
	/// @param input 入力情報
	virtual void InputScene(const InputAction& input) override;
	
	/// @brief 更新処理
    /// アクティブ状態のサブシーン、UIシーンの更新処理を行う
	/// @param deltaTime フレーム間の時間
    virtual void UpdateScene(float deltaTime) override;

	/// @brief 描写処理
	/// アクティブ状態のオブジェクトにアタッチされているスプライトコンポーネントの描写処理を行う
	/// Rendererで呼び出され低解像度レンダリングの処理を行う
	/// @param shader シェーダー
	/// @param bufferLayer 描写レイヤー
    void Render(Shader& shader, int bufferLayer);

	/// @brief UIの描写処理
	/// アクティブ状態のUIシーンの描写処理を行う
	/// Rendererで呼び出され実ウィンドウサイズでの描写処理を行う
	/// @param shader シェーダー
	/// @param bufferLayer 描写レイヤー
	void RenderUIs(Shader& shader);

	
	// ===== シーン管理 ===== //

	void ReloadScene() override;

	/// @brief サブシーンの切り替え
	/// 現在のサブシーンを破棄し、新しいサブシーンを生成する
	/// @tparam T 新しいサブシーンクラス(SubSceneを継承したクラス)
	/// @param args シーンの初期化に必要な引数( 第一引数のメインシーンの参照は自動で設定される )
	template <typename T, typename... Args>
	T& ChangeSubScene(Args&&... args) {
		// 新しいシーンを作成
		std::unique_ptr<T> newScene = std::make_unique<T>(*this, std::forward<Args>(args)...);

		// 初期化
		newScene->Initialize();

		// 古いシーンを破棄して新しいシーンを設定
		_currentSubScene.reset();
		_currentSubScene = std::move(newScene);

		// 適切な型にキャストして返す
		return static_cast<T&>(*_currentSubScene);
	}
	
	/// @brief UISceneの生成
	/// UISceneを生成し、UISceneの初期化を行う
	/// @tparam T 生成するUISceneのクラス(UISceneを継承したクラス)
	/// @param args UISceneのコンストラクタに渡す引数 (第一引数のメインシーンの参照は自動で渡される)
	template <typename T, typename... Args>
    T& CreateUIScene(Args&&... args) {
		std::unique_ptr<T> uiScene = std::make_unique<T>(*this, std::forward<Args>(args)...);
		T& uiSceneRef = *uiScene;
		uiScene->Initialize();
		_uiScenes.push_back(std::move(uiScene));
		return uiSceneRef;
    }

	/// @brief UISceneの削除
	/// UISceneを削除する
	/// UISceneのデストラクタで呼ばれることを想定している
	/// @param uiScene 削除するUIScene
	void RemoveUIScene(UIScene* uiScene);


	// ===== データ管理 ===== //

	/// @brief データの設定
	/// ゲーム全体で使用するデータを設定する
	/// @param key データのキー	
	/// @param value データの初期の値
	void SetData(const std::string& key, GameTypes::DataValue value);

	/// @brief データの更新関数の設定
	/// @param key データのキー
	/// @param update データを取り出す際に駆動する関数 例)ゲッターなど 
	void SetDataUpdate(const std::string& key, std::function<GameTypes::DataValue()> update);

	/// @brief データの取得
	/// @param key データのキー
	/// @return データの値
	GameTypes::DataValue GetData(const std::string& key);


	// ===== ゲッター・セッター ===== //

	Camera* GetCamera() const { return _camera; }
    std::vector<std::unique_ptr<UIScene>>& GetUIScenes() { return _uiScenes; }
    
protected:

	// ===== リソース管理 ===== //

	/// @brief データの読み込み
	/// ゲームの初期化時にデータを読み込む、実装は継承先で行う
    virtual void LoadData() override {}; 
	virtual void LoadObjects() override {}; // ゲームオブジェクトの読み込み

	/// @brief データの解放
	/// ゲームの終了時にデータを解放する、実装は継承先で行う
    virtual void UnloadData() override {}; 
		

	// ===== 終了処理===== //

	/// @brief シャットダウン処理
	/// UnloadData()を呼び出す
	/// 特別なシャットダウン処理が必要な場合はオーバーライドする
    virtual void Shutdown() override;


	// ===== ゲームオブジェクトの管理 ===== //

	Camera* _camera;													///< シーンのカメラ
	std::unique_ptr<SubScene> _currentSubScene;											///< 現在のサブシーン
	std::vector<std::unique_ptr<UIScene>> _uiScenes;									///< UIシーンの配列
	std::unordered_map<std::string, GameTypes::DataValue> _data;						///< ゲーム全体で使用するデータ
	std::unordered_map<std::string, std::function<GameTypes::DataValue()>> _dataUpdate; ///< データの更新関数
};