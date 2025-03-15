#include "GameObjects/GameObject.h"
#include "Components/Component.h"
#include "Utilities/Input.h"
#include "Utilities/Frame.h"
#include "Core/Game.h"
#include <algorithm>
#include "Scene/Scene.h"
#include "Scene/MainScene.h"
#include "Scene/SceneManager.h"

#pragma region con

GameObject::GameObject(Scene* scene)
	: _state(STATE::ACTIVE)
	, _tag(TAG::NONE)
	, _scene(scene)
	, _position(Vector2Int::Zero)
	, _scale(1.0f)
	, _rotation(0.0f)
	, _recomputeWorldTransform(true)
	{
	_scene->AddObject(this);
}

GameObject::~GameObject() {
	_scene->RemoveObject(this);
}

#pragma endregion

#pragma region パブリック関数

void GameObject::ProcessInput(Input* input) {
	
	if (_state == STATE::ACTIVE) {
		for (auto& comp : _components) {
			if (comp->GetState() == Component::STATE::ACTIVE) {
				comp->ProcessInput(input);
			}
		}
		InputObject(input);
	}
}

void GameObject::InputObject(Input* input) {

}

void GameObject::Update(float deltaTime) {

	/*ゲーム側から呼び出される更新関数*/

	if (_state == STATE::ACTIVE)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateObject(deltaTime);

		ComputeWorldTransform();
	}
}

void GameObject::UpdateComponents(float deltaTime) {

	/*つけられたコンポーネントを順に実行する*/

	for (auto comp : _components) {
		if (comp->GetState() == Component::STATE::ACTIVE) {
			comp->Update(deltaTime);
		}
	}
}

void GameObject::UpdateObject(float deltaTime) {
	
}


void GameObject::PhysUpdate(float deltaTime) {

	/*物理更新関数。ゲーム側から呼び出される。*/

	if (_state == STATE::ACTIVE)
	{
		ComputeWorldTransform();

		PhysUpdateComponents(deltaTime);
		PhysUpdateObject(deltaTime);

		ComputeWorldTransform();
	}
}

void GameObject::PhysUpdateComponents(float deltaTime) {

	/*つけられたコンポーネントを順に実行する*/

	for (auto comp : _components) {
		if (comp->GetState() == Component::STATE::ACTIVE) {
			comp->PhysUpdate(deltaTime);
		}
	}
}

void GameObject::PhysUpdateObject(float deltaTime) {

}

void GameObject::Render(Shader* shader, int bufferLayer) {

	/*描画関数。ゲーム側から呼び出される。*/

	if (_state == STATE::ACTIVE) {
		for (auto& comp : _components) {
			if (comp->GetBufferLayer() == bufferLayer && comp->GetState() == Component::STATE::ACTIVE) {
				comp->Render(shader);
			}
		}
	}
}

void GameObject::RenderUI(Shader* shader) {

	/*UI描画関数。ゲーム側から呼び出される。*/

	if (_state == STATE::ACTIVE) {
		for (auto& comp : _components) {
			comp->Render(shader);
		}
	}

}


void GameObject::AddComponent(Component* component) {
	
	/*コンポーネントのupdateLayerの値に応じて昇順に並べる*/

	int layer = component->UpdateLayer();
	auto iter = _components.begin();

	for (; iter != _components.end(); ++iter) {
	
		if (layer < (*iter)->UpdateLayer()) { // 階層数が既存のコンポーネントより小さければ
			break; // 抜ける
		}

	}

	_components.insert(iter, component);

}

void GameObject::RemoveComponent(Component* component) {

	auto comp = std::find(_components.begin(), _components.end(),component);
	if (comp != _components.end()) {
		_components.erase(comp);
	}

}
void GameObject::AddChildren(GameObject* object) {

	_childrenObjects.emplace_back(object);
    
}

void GameObject::RemoveChildren(GameObject* object) {
    auto iter = std::find(_childrenObjects.begin(), _childrenObjects.end(), object);
    if (iter != _childrenObjects.end()) {
        std::iter_swap(iter, _childrenObjects.end() - 1);
		_childrenObjects.pop_back();
    }

}

void GameObject::ComputeWorldTransform() {
	if (_recomputeWorldTransform) {
		_recomputeWorldTransform = false;

		_worldTransform = Matrix4::CreateScale(_scale);
		_worldTransform *= Matrix4::CreateRotationZ(_rotation);
		_worldTransform *= Matrix4::CreateTranslation(Vector3(static_cast<float>(_position.x), static_cast<float>(_position.y), 0.0f));

		for (auto comp : _components) {
			comp->OnUpdateWorldTransform();
		}
	}
}


MainScene* GameObject::GetMainScene() {
	return _scene->GetManager()->GetCurrentMainScene();
}

#pragma endregion

#pragma region プライベート関数

#pragma endregion
