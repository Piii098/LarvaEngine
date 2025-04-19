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


	virtual void ProcessInput(const InputAction& input) {};
	virtual void Update(float deltaTime) {};
	virtual void FixedUpdate(float deltaTime) {};
	virtual void OnUpdateWorldTransform() {};
	
	/*ゲッターセッター*/

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