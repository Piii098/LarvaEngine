#pragma once

// ===== 前方宣言 ===== //
class MainScene;
class InputAction;

/// @brief サブシーンクラス
///
/// メインシーン内のサブシーンの基底クラス
/// メインシーン内のゲームオブジェクトを操作し、具体的なゲームロジックを実装する
/// 例) 入力により、ゲームオブジェクトの移動、攻撃、アイテムの取得など
class SubScene {
public:
	enum class STATE {
		ACTIVE,     // アクティブ
		INACTIVE,   // 非アクティブ
		CLOSE       // 終了
	};

	// ===== コンストラクタ・デストラクタ ===== //

	SubScene(MainScene& parent) : _parent(parent), _state(STATE::ACTIVE) {};
	virtual ~SubScene() {};

	// ===== 初期設定 ===== //
	
	/// @brief シーンの初期化
	/// 仮想関数、初期化処理
	virtual void Initialize() {};

	// ===== ループ処理 ===== //

	/// @brief 入力処理
	/// 仮想関数、入力処理を行う
	/// @param input 入力情報
	virtual void InputScene(const InputAction& input) {};
	
	/// @brief 更新処理
	/// 仮想関数、更新処理を行う
	/// @param deltaTime フレーム間の時間
	virtual void UpdateScene(float deltaTime) {};

	// ===== 終了処理 ===== //

	/// @brief 終了処理
	/// 仮想関数、終了処理を行う
	virtual void Shutdown() {};


	// ===== ゲッター・セッター ===== //

	STATE State() const { return _state; };
	void State(STATE state) { _state = state; };

protected:

	MainScene& _parent; ///< 親メインシーン

	STATE _state;		///< サブシーンの状態
};