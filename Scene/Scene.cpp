#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Core/Game.h"
#include "GameObjects/GameObject.h"
#include "AssetManagers/AssetManager.h"
#include "Utilities/Input.h"
#include <algorithm>
#include "GameObjects/Camera.h"
#include "GameObjects/Player.h"
#include "AssetManagers/AssetData/Texture.h"

#pragma region �R���X�g���N�^:�f�X�g���N�^

Scene::Scene(SceneManager* maneger)
	: _manager(maneger) 
	, _isUpdating(false){
}

Scene::~Scene() {

}

#pragma endregion


#pragma region �p�u���b�N�֐�

void Scene::Initialize() {

	_camera = new Camera(this);

	LoadData();
}

void Scene::Shutdown() {
	UnloadData();
}

void Scene::ProcessInput(Input* input) {
	_isUpdating = true;
	for (auto& obj : _objects) {
		obj->ProcessInput(input);
	}
	_isUpdating = false;
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
	// �e�I�u�W�F�N�g����폜
	if (object->GetParent()) {
		object->GetParent()->RemoveChildren(object);
	}

	// �q�I�u�W�F�N�g���폜
	for (auto child : object->GetChildren()) {
		child->SetParent(nullptr); // �e������
		RemoveObject(child);       // �q�I�u�W�F�N�g���폜
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
    // _pendingObjects����폜
    auto iter = std::find(_pendingObjects.begin(), _pendingObjects.end(), object);
    if (iter != _pendingObjects.end()) {
        delete *iter; // �����������
        *iter = nullptr; // nullptr�ɐݒ�
        _pendingObjects.erase(iter);
    }

    // _objects����폜
    iter = std::find(_objects.begin(), _objects.end(), object);
    if (iter != _objects.end()) {
        delete *iter; // �����������
        *iter = nullptr; // nullptr�ɐݒ�
        _objects.erase(iter);
    }
}

#pragma endregion

#pragma region �v���C�x�[�g�֐�


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