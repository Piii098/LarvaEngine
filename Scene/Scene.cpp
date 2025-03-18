#include "Scene/Scene.h"
#include <algorithm>
#include "UI/UIScene.h"
#include "Components/Draw/SpriteComponent.h"

#pragma region コンスト


Scene::Scene(SceneManager* manager)
	: _manager(manager)
	, _camera(nullptr)
	, _state(STATE::ACTIVE)
	, _isUpdating(false) {
}

Scene::~Scene() {
}

#pragma endregion


#pragma region パブリック関数

void Scene::ProcessInput(Input* input) {

	if (_state == STATE::ACTIVE) {

		_isUpdating = true;
		for (auto& obj : _objects) {
			obj->ProcessInput(input);
		}
		_isUpdating = false;
	
	}

	InputScene(input);

}

void Scene::Update(float deltaTime) {

	_isUpdating = true;
	for (auto& obj : _objects) {
		obj->Update(deltaTime);
	}
	_isUpdating = false;

	UpdateScene(deltaTime);

	for (auto pend : _pendingObjects) {
		pend->ComputeWorldTransform();
		_objects.emplace_back(pend);
	}
	_pendingObjects.clear();

	// 死亡オブジェクトを削除
	std::vector<GameObject*> deadObjects;
	for (auto obj : _objects) {
		if (obj->State() == GameObject::STATE::DEAD) {
			deadObjects.emplace_back(obj);
		}
	}

	for (auto obj : deadObjects) {
		delete obj;
	}
	_objects.erase(std::remove_if(_objects.begin(), _objects.end(),
		[](GameObject* obj) { return obj->State() == GameObject::STATE::DEAD; }), _objects.end());

}


void Scene::PhysUpdate(float deltaTime) {

	for (auto& obj : _objects) {
		obj->PhysUpdate(deltaTime);
	}

}

void Scene::Output() {

}

void Scene::AddObject(GameObject* object) {
	if (_isUpdating) {
		_pendingObjects.emplace_back(object);
	}
	else {
		_objects.emplace_back(object);
	}
}

void Scene::RemoveObject(GameObject* object) {
	// 親オブジェクトから削除
	if (object->GetParent()) {
		object->GetParent()->RemoveChildren(object);
	}

	// 子オブジェクトを削除
	for (auto child : object->GetChildren()) {
		child->SetParent(nullptr); // 親を解除
		RemoveObject(child);       // 子オブジェクトを削除
	}

	auto iter = std::find(_pendingObjects.begin(), _pendingObjects.end(), object);
	if (iter != _pendingObjects.end()) {
		std::iter_swap(iter, _pendingObjects.end() - 1);
		_pendingObjects.pop_back();
	}

	iter = std::find(_objects.begin(), _objects.end(), object);
	if (iter != _objects.end()) {
		std::iter_swap(iter, _objects.end() - 1);
		_objects.pop_back();
	}
}

void Scene::DestroyObject(GameObject* object) {
	// _pendingObjectsから削除
	auto iter = std::find(_pendingObjects.begin(), _pendingObjects.end(), object);
	if (iter != _pendingObjects.end()) {
		delete* iter; // メモリを解放
		*iter = nullptr; // nullptrに設定
		_pendingObjects.erase(iter);
	}

	// _objectsから削除
	iter = std::find(_objects.begin(), _objects.end(), object);
	if (iter != _objects.end()) {
		delete* iter; // メモリを解放
		*iter = nullptr; // nullptrに設定
		_objects.erase(iter);
	}
}


void Scene::AddSprite(SpriteComponent* sprite) {
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->DrawLayer();
	auto iter = _sprites.begin();
	for (;
		iter != _sprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->DrawLayer())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	_sprites.insert(iter, sprite);
}

void Scene::RemoveSprite(SpriteComponent* sprite) {
	auto iter = std::find(_sprites.begin(), _sprites.end(), sprite);
	if (iter != _sprites.end()) {
		_sprites.erase(iter);
	}
}

void Scene::UnloadData() {
	for (auto& obj : _objects) {
		if (obj != nullptr) {
			delete obj;
			obj = nullptr;
		}
	}
	_objects.clear();
}

void Scene::Shutdown() {
	UnloadData();
}



#pragma endregion