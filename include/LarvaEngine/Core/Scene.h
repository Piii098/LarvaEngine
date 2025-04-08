#pragma once
#include <vector>
#include <memory>
#include "LarvaEngine/Core/GameObject.h"

#include "LarvaEngine/Core/SubScene.h"

// ===== 前方宣言 =====
class SceneManager;
class Camera;
class UIScreen;
class Shader;
class UIScene;
class SpriteComponent;
class InputSystem;
class InputAction;


/// @brief シーンクラス
/// 
/// シーンの基底クラス
/// ゲームオブジェクトの管理、生成、破棄を行う
/// スプライトの管理も行う
/// StateがCLOSEの場合、シーンを終了、削除される
class Scene {
public:

    enum class STATE {
		ACTIVE,     // アクティブ
		INACTIVE,   // 非アクティブ
		PAUSE,      // 一時停止(描写する)
		CLOSE       // 終了
    };

    Scene(SceneManager& manager);

    virtual ~Scene();


	// ===== 初期設定 ===== //

    virtual void Initialize() {};
    

	// ===== ループ処理 ===== //

	/// @brief 入力処理
	/// ゲームオブジェクト、シーンの入力処理を行う
	/// @param input 入力情報
    void ProcessInput(const InputAction& input);

	/// @brief 更新処理
	/// ゲームオブジェクト、シーンの更新処理を行う
	/// @param deltaTime フレーム間の時間
    void Update(float deltaTime);

	/// @brief 物理更新処理
	/// ゲームオブジェクトの物理更新処理を行う
	/// @param deltaTime 補正されたフレーム間の時間
    void PhysUpdate(float deltaTime);

	/// @brief 出力処理
	/// ゲームオブジェクトの出力処理を行う
	/// @param deltaTime フレーム間の時間
    void Output();


	// ===== ゲームオブジェクト管理 ===== //

	/// @brief ゲームオブジェクトの生成
	/// ゲームオブジェクトを生成し、配列に追加する
	/// @tparam T 生成するゲームオブジェクトのクラス
	/// @param args ゲームオブジェクトのコンストラクタに渡す引数 (第一引数のシーンの参照は自動で渡される)
	/// @return 生成したゲームオブジェクトのポインタ
    template <typename T, typename... Args>
    T& CreateGameObject(Args&&... args) {
		std::unique_ptr<T> object = std::make_unique<T>(*this, std::forward<Args>(args)...);
		T& objRef = *object;
		if (_isUpdating) {
			_pendingObjects.emplace_back(std::move(object));
		}
		else {
			_objects.emplace_back(std::move(object));
		}
		return objRef;
    }

	/// @brief 子オブジェクトの生成
	/// 親オブジェクトに子オブジェクトを生成し、配列に追加する
	/// @tparam T 生成するゲームオブジェクトのクラス
	/// @param parent 親オブジェクト
	/// @param args ゲームオブジェクトのコンストラクタに渡す引数 (第一引数のシーンの参照は自動で渡される)
	/// @return 生成したゲームオブジェクトのポインタ
    template <typename T, typename... Args>
    T& CreateChildObject(GameObject* parent, Args&&... args) {
		std::unique_ptr<T> object = std::make_unique<T>(parent, std::forward<Args>(args)...);
		T& objRef = *object;
		if (_isUpdating) {
			_pendingObjects.emplace_back(std::move(object));
		}
		else {
			_objects.emplace_back(std::move(object));
		}
		// parent->AddChild(&objRef);
		return objRef;
    }

	/// @brief ゲームオブジェクトの削除
	/// シーンのゲームオブジェクトの配列からゲームオブジェクトを削除する
	/// 子オブジェクトを持つ場合は子オブジェクトも削除される
	/// GameObjectのデストラクタで呼ばれることを想定
	/// オブジェクトを破棄する場合はDestroyObjectを使用する
	/// @param object 削除するゲームオブジェクト
    void RemoveObject(GameObject* object);

	/// @brief ゲームオブジェクトの破棄
	/// ゲームオブジェクトを削除する
	/// オブジェクトを破棄する場合はこれを使用する
	/// @param object 破棄するゲームオブジェクト
	void DestroyObject(GameObject* object);


	// ===== スプライト関連 ===== //

	/// @brief スプライトの追加
	/// シーンのスプライトの配列にスプライトを追加する
	/// @param sprite 追加するスプライト
	void AddSprite(SpriteComponent* sprite);

	/// @brief スプライトの削除
	/// シーンのスプライトの配列からスプライトを削除する
	/// @param sprite 削除するスプライト
	void RemoveSprite(SpriteComponent* sprite);


	// ===== ゲッター・セッター =====//

    SceneManager& GetManager() { return _manager; }
    Camera& GetCamera() { return *_camera.get(); }
	STATE State() { return _state; }
	const SceneManager& GetManager() const { return _manager; }
	const Camera& GetCamera() const { return *_camera.get(); }
	const STATE State() const { return _state; }

	void State(STATE state) { _state = state; }

protected:

	// ===== リソース管理 ===== //

	/// @brief データの読み込み
	/// シーンのデータの読み込みを行う (継承先で実装)
    virtual void LoadData() {}; // データの読み込み

	/// @brief データの解放
	/// シーンのデータの解放を行う (継承先で実装)
    virtual void UnloadData(); // データの解放


	// ===== ループ処理 ===== //

    /// @brief 独自入力処理
	/// シーンの入力処理を行う (継承先で実装)
    /// @param input 入力情報
    virtual void InputScene(const InputAction& input) {};

	/// @brief 独自更新処理
	/// シーンの更新処理を行う　(継承先で実装)
	/// @param deltaTime フレーム間の時間
    virtual void UpdateScene(float deltaTime) {};

	// ===== ゲームオブジェクト ===== //

	/// @brief ゲームオブジェクトの追加
	/// シーンのゲームオブジェクトの配列にゲームオブジェクトを追加する
	/// @param object 追加するゲームオブジェクト
	void AddObject(GameObject* object);

	// ===== 終了処理 ===== //

	/// @brief  終了処理
	/// シーンの終了処理を行い、デストラクタで呼ばれる
	/// UnloadData()を実装済み
	/// 特別な終了処理が必要な場合は継承先で実装
	/// @param deltaTime フレーム間の時間
    virtual void Shutdown();

	
	// ===== ゲームオブジェクト ===== //
	std::vector<std::unique_ptr<GameObject>> _pendingObjects; ///< 追加予定のオブジェクト
	std::vector<std::unique_ptr<GameObject>> _objects;        ///< ゲームオブジェクトの配列
	

	// ===== スプライト ===== //
	std::vector<SpriteComponent*> _sprites;	  ///< スプライトの配列

	std::unique_ptr<Camera> _camera; ///< シーンのカメラ
	SceneManager& _manager;			 ///< シーンマネージャー	
	
	STATE _state;					 ///< シーンの状態
	bool _isUpdating;				 ///< 更新中かどうか	

};