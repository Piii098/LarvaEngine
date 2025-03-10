﻿#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Core/Game.h"
#include "GameObjects/GameObject.h"
#include "AssetManagers/AssetManager.h"
#include "Utilities/Input.h"
#include <algorithm>
#include "GameObjects/Camera.h"
#include "GameObjects/Player.h"
#include "AssetManagers/AssetData/Texture.h"
#include "UI/UIScreen.h"

#pragma region コンストラクタ:デストラクタ

Scene::Scene(SceneManager* maneger)
	: _manager(maneger) 
	, _isUpdating(false)
	, _state(STATE::GAME_PLAY)
	, _camera(nullptr){
}

Scene::~Scene() {

}

#pragma endregion


#pragma region パブリック関数

void Scene::Initialize() {

	_camera = new Camera(this);

	LoadData();
}

void Scene::Shutdown() {
	UnloadData();
}

void Scene::ProcessInput(Input* input) {
	switch (_state) {
	case STATE::GAME_PLAY:
		_isUpdating = true;
		for (auto& obj : _objects) {
			obj->ProcessInput(input);
		}
		_isUpdating = false;
		break;
	case STATE::PAUSE:
		_uiScreens.back()->ProcessInput(input);
		break;
	default:

		break;
	}

	InputScene(input);
}

void Scene::Update(float deltaTime) {

	_isUpdating = true;
	for (auto& obj : _objects) {
		obj->Update(deltaTime);
	}
	_isUpdating = false;

	for (auto pend : _pendingObjects) {
		pend->ComputeWorldTransform();
		_objects.emplace_back(pend);
	}
	_pendingObjects.clear();

	UpdateScene(deltaTime);

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

	for (auto ui : _uiScreens) {
		if (ui->State() == UIScreen::STATE::ACTIVE) {
			ui->Update(deltaTime);
		}
	}

	auto iter = _uiScreens.begin();
	while (iter != _uiScreens.end()) {
		if ((*iter)->State() == UIScreen::STATE::CLOSING) {
			delete *iter;
			*iter = nullptr;
			iter = _uiScreens.erase(iter);
		}
		else {
			++iter;
		}
	}
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
        delete *iter; // メモリを解放
        *iter = nullptr; // nullptrに設定
        _pendingObjects.erase(iter);
    }

    // _objectsから削除
    iter = std::find(_objects.begin(), _objects.end(), object);
    if (iter != _objects.end()) {
        delete *iter; // メモリを解放
        *iter = nullptr; // nullptrに設定
        _objects.erase(iter);
    }
}

void Scene::PushUI(UIScreen* screen) {
	_uiScreens.emplace_back(screen);
}

#pragma endregion

#pragma region プライベート関数


void Scene::LoadData() {
	
}

void Scene::UnloadData() {
	for (auto& obj : _objects) {
		delete obj;
		obj = nullptr;
	}
	_objects.clear();
}

#pragma endregion