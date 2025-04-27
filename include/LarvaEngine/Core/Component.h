#pragma once

// ===== 前方宣言 =====//
class GameObject;
class Frame;
class Shader;
class InputAction;

/// @brief ゲームオブジェクトのコンポーネントクラス
///	
/// ゲームオブジェクトにアタッチされるコンポーネントの基本クラス
class Component {
public:

	enum class STATE {
		ACTIVE,
		INACTIVE,
		DEAD
	};

	// ===== コンストラクタ・デストラクタ ===== //

	Component(GameObject& parent, int updateLayer = 100);
	virtual ~Component();

	virtual void CleanUp() {};
	
	// ===== ループ処理 ===== //

	/// @brief 入力処理
	/// コンポーネントの入力処理を行う
	/// /// @param input 入力情報
	virtual void ProcessInput(const InputAction& input) {};

	/// @brief 更新処理
	/// コンポーネントの更新処理を行う
	/// @param deltaTime フレーム間の時間
	virtual void Update(float deltaTime) {};
	
	/// @brief 物理更新処理
	/// コンポーネントの物理更新処理を行う
	/// /// @param deltaTime フレーム間の時間
	virtual void FixedUpdate(float deltaTime) {};

	/// @brief ワールド変換行列の計算
	/// コンポーネントのワールド変換行列を計算する
	virtual void OnUpdateWorldTransform() {};
	
	// ===== ゲッターセッター ===== //

	int GetBufferLayer() const { return _bufferLayer; };
	void SetBufferLayer(int bufferLayer) { _bufferLayer = bufferLayer; };
	int GetRenderType() const { return _renderType; };
	GameObject& GetParent() const { return _parent; };
	int UpdateLayer() const { return _updateLayer; };

	STATE State() const { return _state; };
	void State(STATE state) { _state = state; };

protected:
	STATE _state;
	GameObject& _parent;
	int _updateLayer;
	int _bufferLayer;
	int _renderType;
};