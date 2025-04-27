#include <algorithm>
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/MainScene.h"
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/SceneManager.h"

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

/**
 * コンストラクタ
 * 
 * 親オブジェクトを持たない
 */
GameObject::GameObject(Scene& scene)
	: _state(STATE::ACTIVE)
	, _tag(TAG::NONE)
	, _scene(scene)
	, _position(Vector2Int::Zero)
	, _scale(1.0f)
	, _rotation(0.0f)
	, _recomputeWorldTransform(true){
	//SetParent(nullptr);
}

/**
 * デストラクタ
 * 
 * 親オブジェクトから自身を削除する
 */
GameObject::~GameObject() {
	//_scene.RemoveObject(this);
}


//==============================================================================
// パブリック関数
//==============================================================================

void GameObject::Cleanup() {
	// コンポーネントのクリーンアップ処理を呼び出す
	for (auto& comp : _components) {
		//comp->Cleanup();
	}

}

// ===== ループ処理 ===== //

/**
 * 入力処理
 * 
 * コンポーネントと独自の入力処理を呼び出す
 * アクティブ状態でなければ処理しない
 */
void GameObject::ProcessInput(const InputAction& input) {
	
	if (_state != STATE::ACTIVE) return;

	InputComponents(input);
	InputObject(input);
}

/**
 * 更新処理
 * 
 * コンポーネントと独自の更新処理を呼び出し、ワールド変換行列を計算する
 * アクティブ状態でなければ処理しない
 */
void GameObject::Update(const float deltaTime) {

	if (_state != STATE::ACTIVE) return;

	ComputeWorldTransform();

	UpdateComponents(deltaTime);
	UpdateObject(deltaTime);

	ComputeWorldTransform();

}

/**
 * 物理更新処理
 * 
 * コンポーネントと独自の物理更新処理を呼び出し、ワールド変換行列を計算する
 * アクティブ状態でなければ処理しない
 */
void GameObject::FixedUpdate(const float deltaTime) {

	if (_state != STATE::ACTIVE) return;

	ComputeWorldTransform();

	FixedUpdateComponents(deltaTime);
	FixedUpdateObject(deltaTime);

	ComputeWorldTransform();

}

// ====== コンポーネント関連 ===== //

/**
 * コンポーネントの削除
 * 
 * コンポーネントを検索し削除する
 */
void GameObject::RemoveComponent(Component* component) {
	/*
	// 検索
	auto comp = std::find_if(_components.begin(), _components.end(),
		[component](const std::unique_ptr<Component>& comp) {
			return comp.get() == component; // ポインタが一致するか
		});

	if (comp != _components.end()) {
		_components.erase(comp);
	}
	*/
}

// ====== ワールド変換行列関連 ===== //

/**
 * ワールド変換行列の計算
 * 位置、スケール、回転からワールド変換行列を計算する
 */
void GameObject::ComputeWorldTransform() {

	if (_recomputeWorldTransform) { // 再計算フラグが立っている場合のみ計算
		_recomputeWorldTransform = false;

		_worldTransform = Matrix4::CreateScale(_scale);
		_worldTransform *= Matrix4::CreateRotationZ(_rotation);
		_worldTransform *= Matrix4::CreateTranslation(Vector3(static_cast<float>(_position.x), static_cast<float>(_position.y), 0.0f));

		// 子オブジェクトのワールド変換行列も再計算
		for (auto& comp : _components) {
			comp->OnUpdateWorldTransform();
		}
	}
}

// ====== ゲッターセッター ===== //

MainScene& GameObject::GetMainScene() {
	return _scene.GetManager().GetCurrentMainScene();
}



//==============================================================================
// プライベート関数
//==============================================================================

/**
 * コンポーネントの追加
 *
 * コンポーネントのupdateLayerを取得し、その値を比較して挿入する
 * 所有権は移動する
 */
void GameObject::AddComponent(std::unique_ptr<Component> component) {

	/*コンポーネントのupdateLayerの値に応じて昇順に並べる*/

	int layer = component->UpdateLayer();
	auto iter = _components.begin();

	for (; iter != _components.end(); ++iter) {

		if (layer < (*iter)->UpdateLayer()) { // 階層数が既存のコンポーネントより小さければ
			break; // 抜ける
		}

	}

	//_components.insert(iter, std::move(component));

}


// ====== ループ処理 ===== //

/**
 * 更新処理
 * 
 * コンポーネントの更新処理を呼び出す
 */
void GameObject::InputComponents(const InputAction& input) {


	for (auto& comp : _components) {
		if (comp->State() == Component::STATE::ACTIVE) { // アクティブ状態のコンポーネントのみ入力処理
			comp->ProcessInput(input);
		}
	}
}

/**
 * 更新処理
 *
 * コンポーネントの更新処理を呼び出す
 */
void GameObject::UpdateComponents(const float deltaTime) {

	for (auto& comp : _components) {
		if (comp->State() == Component::STATE::ACTIVE) { // アクティブ状態のコンポーネントのみ更新
			comp->Update(deltaTime);
		}
	}
}

/**
 * 物理更新処理
 *
 * コンポーネントの物理更新処理を呼び出す
 */
void GameObject::FixedUpdateComponents(const float deltaTime) {

	for (auto& comp : _components) {
		if (comp->State() == Component::STATE::ACTIVE) { // アクティブ状態のコンポーネントのみ更新
			comp->FixedUpdate(deltaTime);
		}
	}
}