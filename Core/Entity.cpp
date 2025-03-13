#include "Core/Entity.h"
#include "Components/Component.h"
#include "Scene/Scene.h"

Entity::Entity(Scene* scene)
    : _scene(scene)
	, _state(STATE::ACTIVE)
	, _position(Vector2Int::Zero)
	, _scale(1.0f)
	, _rotation(0.0f)
	, _recomputeWorldTransform(true) {
}

Entity::~Entity() {
    _components.clear();
}

void Entity::ProcessInput(Input* input) {

	if (_state == STATE::ACTIVE) {
		InputComponents(input);
		InputEntity(input);
	}
}

void Entity::InputComponents(Input* input) {
	for (auto& comp : _components) {
		comp->ProcessInput(input);
	}
}

void Entity::Update(float deltaTime) {

	if (_state == STATE::ACTIVE)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateEntity(deltaTime);

		ComputeWorldTransform();
	}
}

void Entity::UpdateComponents(float deltaTime) {
	for (auto comp : _components) {
		comp->Update(deltaTime);
	}
}

void Entity::Render(Shader* shader) {
	RenderComponents(shader);
}

void Entity::RenderComponents(Shader* shader) {
	for (auto comp : _components) {
		comp->Render(shader);
	}
}

void Entity::AddComponent(Component* component) {

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

void Entity::RemoveComponent(Component* component) {
	auto comp = std::find(_components.begin(), _components.end(), component);
	if (comp != _components.end()) {
		_components.erase(comp);
	}
}

void Entity::ComputeWorldTransform() {
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