#include "GameObjects/GameObject.h"
#include "Components/Component.h"
#include "Utilities/Input.h"
#include "Utilities/Frame.h"
#include "Game.h"

#pragma region con


GameObject::GameObject(Game* game)
	: _state(STATE::ACTIVE)
	, _tag(TAG::NONE)
	, _game(game)
	, _position(Vector2::Zero)
	, _scale(1.0f)
	, _rotation(0.0f)
	, _recomputeWorldTransform(true)
	{
	_game->AddObject(this);
}

GameObject::~GameObject() {
	
}


#pragma endregion

#pragma region パブリック関数

void GameObject::ProcessInput(Input* input) {
	
	if (_state == STATE::ACTIVE) {
		for (auto& comp : _components) {
			comp->ProcessInput(input);
		}
		InputObject(input);
	}
}

void GameObject::InputObject(Input* input) {

}

void GameObject::PhysUpdate(Frame* frame) {

}

void GameObject::Update(Frame* frame) {

	/*ゲーム側から呼び出される更新関数*/

	if (_state == STATE::ACTIVE)
	{
		ComputeWorldTransform();

		UpdateComponents(frame);
		UpdateObject(frame);

		ComputeWorldTransform();
	}
}

void GameObject::UpdateComponents(Frame* frame) {

	/*つけられたコンポーネントを順に実行する*/

	for (auto comp : _components) {
		comp->Update(frame);
	}
}

void GameObject::UpdateObject(Frame* frame) {

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

void GameObject::ComputeWorldTransform() {

	if (_recomputeWorldTransform) {

		_recomputeWorldTransform = false;

		_worldTransform = Matrix4::CreateScale(_scale);
		_worldTransform *= Matrix4::CreateRotationZ(_rotation);
		_worldTransform *= Matrix4::CreateTranslation(Vector3(_position.x, _position.y, 0.0f));

		for(auto comp : _components) {
			comp->OnUpdateWorldTransform();
		}

	}
}

#pragma endregion

#pragma region プライベート関数

#pragma endregion
