#include <algorithm>
#include "LarvaEngine/Core/UIScene.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include <iterator>

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

Scene::Scene(SceneManager& manager)
	: _manager(manager)
	, _state(STATE::ACTIVE)
	, _isUpdating(false) {
}

Scene::~Scene() {
	Shutdown();
}


//==============================================================================
// パブリック関数
//==============================================================================

/*
 * 入力処理
 * 
 * シーンがアクティブ状態の場合、独自の入力処理を行う
 * シーンがアクティブ状態かつ、ゲームオブジェクトがアクティブ状態の場合
 * ゲームオブジェクトの入力処理を行う
 */
void Scene::ProcessInput(const InputAction& input) {

	if (_state != STATE::ACTIVE) return;

	_isUpdating = true;
	for (auto& obj : _objects) {
		obj->ProcessInput(input);
	}
	_isUpdating = false;
	
	InputScene(input);

}

/*
 * 更新処理
 *
 * シーンがアクティブ状態の場合、独自の更新処理を行う
 * シーンがアクティブ状態かつ、ゲームオブジェクトがアクティブ状態の場合
 * ゲームオブジェクトの更新処理を行う
 * 
 * 待機中のオブジェクトを_objectsに追加
 * デッド状態のオブジェクトを_objectsからdeadObjectsに移動し、削除
 */
void Scene::Update(const float deltaTime) {

	// 更新処理
	if (_state == STATE::ACTIVE) { // アクティブ状態の場合
 
		_isUpdating = true; // 更新中フラグを立てる
		for (auto& obj : _objects) { 
			obj->Update(deltaTime);
		}
		_isUpdating = false;

		UpdateScene(deltaTime); // 独自の更新処理を行う
	}

	// 待機中のオブジェクトを_objectsに追加
	for (auto& pend : _pendingObjects) {
		pend->ComputeWorldTransform();
	}

	// 次にすべてのオブジェクトを一括で移動
	if (!_pendingObjects.empty()) {
		std::move(_pendingObjects.begin(), _pendingObjects.end(),
			std::back_inserter(_objects));
		_pendingObjects.clear(); // 移動後は明示的にクリア
	}

}


/*
 * 物理更新処理
 *
 * シーンがアクティブ状態の場合、独自の物理更新処理を行う
 * シーンがアクティブ状態かつ、ゲームオブジェクトがアクティブ状態の場合
 * ゲームオブジェクトの物理更新処理を行う
 */
void Scene::FixedUpdate(const float deltaTime) {

	if (_state != STATE::ACTIVE) return;

	for (auto& obj : _objects) {
		obj->FixedUpdate(deltaTime);
	}

}

void Scene::LateUpdate(float deltaTime) {
	if (_state != STATE::ACTIVE) return;

	for (auto& obj : _objects) {
		obj->LateUpdate(deltaTime);
	}
	
	LateUpdateScene(deltaTime);
}

void Scene::Output() {

}

void Scene::ReloadScene() {
	_pendingObjects.clear();
	_objects.clear();

}

// ====== オブジェクト関連 ====== //

/*
 * オブジェクトの削除
 *
 * 親オブジェクトから削除
 * 子オブジェクトを削除
 * _pendingObjectsから削除
 * _objectsから削除
 * デストラクタで呼ばれることを想定
 */
void Scene::RemoveObject(GameObject* object) {


}

/*
 * オブジェクトの破棄
 *
 * _pendingObjectsから削除
 * _objectsから削除
 * オブジェクトを破棄する場合はDestroyObjectを使用する
 * デストラクタからRemoveObjectが呼ばれる
 * (明示的解放のみオブジェクトの削除処理が必要)
 */
void Scene::DestroyObject(GameObject* object) {

	// _pendingObjectsから削除
	auto pendingIter = std::find_if(_pendingObjects.begin(), _pendingObjects.end(),
		[object](const std::unique_ptr<GameObject>& ptr) {
			return ptr.get() == object;
		});

	if (pendingIter != _pendingObjects.end()) {
		_pendingObjects.erase(pendingIter);
	}

	// _objectsから削除
	auto objectsIter = std::find_if(_objects.begin(), _objects.end(),
		[object](const std::unique_ptr<GameObject>& ptr) {
			return ptr.get() == object;
		});

	if (objectsIter != _objects.end()) {
		_objects.erase(objectsIter);
	}
}


// ====== スプライト関連 ====== //

/*
 * スプライトの追加
 * 
 * 描画順に従ってソートされた配列に挿入
 * 小さい値が先頭になるように挿入
 */
void Scene::AddSprite(SpriteComponent* sprite) {
	
	// 描写順数値を取得
	int drawLayer = sprite->DrawLayer();
	auto iter = _sprites.begin();
	for (;iter != _sprites.end();++iter){ 
		if (drawLayer < (*iter)->DrawLayer()) // 描写順数値が既存のスプライトより小さければ
		{
			break;
		}
	}

	// 挿入
	_sprites.insert(iter, sprite);
}

/*
 * スプライトの削除
 *
 * スプライトの配列から削除
 */
void Scene::RemoveSprite(SpriteComponent* sprite) {
	auto iter = std::find(_sprites.begin(), _sprites.end(), sprite);
	if (iter != _sprites.end()) {
		_sprites.erase(iter);
	}
}


// ==============================================================================
// プロテクテッド関数
// ==============================================================================

/*
 * オブジェクトの追加
 *
 * 更新中の場合、_pendingObjectsに追加
 * それ以外の場合、_objectsに追加
 */
void Scene::AddObject(GameObject* object) {
	if (_isUpdating) {
		_pendingObjects.emplace_back(object);
	}
	else {
		_objects.emplace_back(object);
	}
}

/*
 * データの解放
 *
 * シーンのデータの解放を行う
 * ゲームオブジェクトの解放はGameObjectのデストラクタで行う
 */
void Scene::UnloadData() {
	
}

/*
 * 終了処理
 *
 * シーンの終了処理を行う
 * デストラクタで呼ばれる
 * UnloadData()を実装済み
 * 特別な終了処理が必要な場合は継承先で実装
 */
void Scene::Shutdown() {
	UnloadData();
}
