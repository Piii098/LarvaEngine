#include <algorithm>
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/MainScene.h"
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/3D/GameObject.h"
#include "LarvaEngine/Core/SceneManager.h"

//==============================================================================
// �R���X�g���N�^�E�f�X�g���N�^
//==============================================================================

/**
 * �R���X�g���N�^
 *
 * �e�I�u�W�F�N�g�������Ȃ�
 */
GameObject::GameObject(Scene& scene)
	: _state(STATE::ACTIVE)
	, _tag(TAG::NONE)
	, _scene(scene)
	, _position(Vector2Int::Zero)
	, _scale(1.0f)
	, _rotation(0.0f)
	, _recomputeWorldTransform(true) {
	SetParent(nullptr);
}

GameObject::GameObject(GameObject* parent)
	: _state(STATE::ACTIVE)
	, _tag(TAG::NONE)
	, _scene(parent->_scene)
	, _position(Vector2Int::Zero)
	, _scale(1.0f)
	, _rotation(0.0f)
	, _recomputeWorldTransform(true) {
	parent->AddChildObject(this);
	SetParent(parent);
}

/**
 * �f�X�g���N�^
 *
 * �e�I�u�W�F�N�g���玩�g���폜����
 */
GameObject::~GameObject() {
	//_scene.RemoveObject(this);

}


//==============================================================================
// �p�u���b�N�֐�
//==============================================================================

void GameObject::Cleanup() {
	// �R���|�[�l���g�̃N���[���A�b�v�������Ăяo��
	for (auto& comp : _components) {
		//comp->Cleanup();
	}

}

// ===== ���[�v���� ===== //

/**
 * ���͏���
 *
 * �R���|�[�l���g�ƓƎ��̓��͏������Ăяo��
 * �A�N�e�B�u��ԂłȂ���Ώ������Ȃ�
 */
void GameObject::ProcessInput(const InputAction& input) {

	if (_state != STATE::ACTIVE) return;

	InputComponents(input);
	InputObject(input);
}

/**
 * �X�V����
 *
 * �R���|�[�l���g�ƓƎ��̍X�V�������Ăяo���A���[���h�ϊ��s����v�Z����
 * �A�N�e�B�u��ԂłȂ���Ώ������Ȃ�
 */
void GameObject::Update(const float deltaTime) {

	if (_state != STATE::ACTIVE) return;

	ComputeWorldTransform();

	UpdateComponents(deltaTime);
	UpdateObject(deltaTime);

	ComputeWorldTransform();

}

/**
 * �����X�V����
 *
 * �R���|�[�l���g�ƓƎ��̕����X�V�������Ăяo���A���[���h�ϊ��s����v�Z����
 * �A�N�e�B�u��ԂłȂ���Ώ������Ȃ�
 */
void GameObject::FixedUpdate(const float deltaTime) {

	if (_state != STATE::ACTIVE) return;

	ComputeWorldTransform();

	FixedUpdateComponents(deltaTime);
	FixedUpdateObject(deltaTime);

	ComputeWorldTransform();

}

void GameObject::LateUpdate(const float deltaTime) {
	if (_state != STATE::ACTIVE) return;

	LateUpdateConponents(deltaTime);
	LateUpdateObject(deltaTime);
}

// ====== �R���|�[�l���g�֘A ===== //

/**
 * �R���|�[�l���g�̍폜
 *
 * �R���|�[�l���g���������폜����
 */
void GameObject::RemoveComponent(Component* component) {
	/*
	// ����
	auto comp = std::find_if(_components.begin(), _components.end(),
		[component](const std::unique_ptr<Component>& comp) {
			return comp.get() == component; // �|�C���^����v���邩
		});

	if (comp != _components.end()) {
		_components.erase(comp);
	}
	*/
}

void GameObject::DestroyComponent(Component* component) {
	auto iter = std::find_if(_components.begin(), _components.end(),
		[component](const std::unique_ptr<Component>& comp) {
			return comp.get() == component; // �|�C���^����v���邩
		});
	if (iter != _components.end()) {
		_components.erase(iter);
	}
}

void GameObject::AddChildObject(GameObject* child) {
	_children.push_back(child);
}

void GameObject::RemoveChildObject(GameObject* child) {

	auto iter = std::find(_children.begin(), _children.end(), child);
	if (iter != _children.end()) {
		_children.erase(iter);
	}
}

void GameObject::Destroy() {
	if (_parent != nullptr) {
		_parent->RemoveChildObject(this);
	}
	for (auto& child : _children) {
		child->Destroy();
	}
	_scene.DestroyObject(this);
}

// ====== ���[���h�ϊ��s��֘A ===== //

/**
 * ���[���h�ϊ��s��̌v�Z
 * �ʒu�A�X�P�[���A��]���烏�[���h�ϊ��s����v�Z����
 */
void GameObject::ComputeWorldTransform() {
	if (_recomputeWorldTransform) { // �Čv�Z�t���O�������Ă���ꍇ�̂݌v�Z
		_recomputeWorldTransform = false;

		// ���g�̃��[���h�ϊ��s����v�Z
		_worldTransform = Matrix4::CreateScale(_scale);
		_worldTransform *= Matrix4::CreateRotationZ(_rotation);
		_worldTransform *= Matrix4::CreateTranslation(Vector3(static_cast<float>(_position.x), static_cast<float>(_position.y), 0.0f));

		// �e�I�u�W�F�N�g������ꍇ�͐e�̃��[���h�ϊ��s�����Z
		if (_parent != nullptr) {
			_worldTransform *= _parent->WorldTransform();
		}

		// ���ׂẴR���|�[�l���g�̃��[���h�ϊ��s����X�V
		for (auto& comp : _components) {
			comp->OnUpdateWorldTransform();
		}

		// �q�I�u�W�F�N�g�̃��[���h�ϊ��s����Čv�Z
		for (auto& child : _children) {
			// �q�I�u�W�F�N�g�ɍČv�Z�t���O�𗧂Ă�
			child->_recomputeWorldTransform = true;
			// �q�I�u�W�F�N�g�̕ϊ��s����Čv�Z
			child->ComputeWorldTransform();
		}
	}
}

// ====== �Q�b�^�[�Z�b�^�[ ===== //

MainScene& GameObject::GetMainScene() {
	return _scene.GetManager().GetCurrentMainScene();
}



//==============================================================================
// �v���C�x�[�g�֐�
//==============================================================================

/**
 * �R���|�[�l���g�̒ǉ�
 *
 * �R���|�[�l���g��updateLayer���擾���A���̒l���r���đ}������
 * ���L���͈ړ�����
 */
void GameObject::AddComponent(std::unique_ptr<Component> component) {

	/*�R���|�[�l���g��updateLayer�̒l�ɉ����ď����ɕ��ׂ�*/

	int layer = component->UpdateLayer();
	auto iter = _components.begin();

	for (; iter != _components.end(); ++iter) {

		if (layer < (*iter)->UpdateLayer()) { // �K�w���������̃R���|�[�l���g��菬�������
			break; // ������
		}

	}

	//_components.insert(iter, std::move(component));

}


// ====== ���[�v���� ===== //

/**
 * �X�V����
 *
 * �R���|�[�l���g�̍X�V�������Ăяo��
 */
void GameObject::InputComponents(const InputAction& input) {


	for (auto& comp : _components) {
		if (comp->State() == Component::STATE::ACTIVE) { // �A�N�e�B�u��Ԃ̃R���|�[�l���g�̂ݓ��͏���
			comp->ProcessInput(input);
		}
	}
}

/**
 * �X�V����
 *
 * �R���|�[�l���g�̍X�V�������Ăяo��
 */
void GameObject::UpdateComponents(const float deltaTime) {

	for (auto& comp : _components) {
		if (comp->State() == Component::STATE::ACTIVE) { // �A�N�e�B�u��Ԃ̃R���|�[�l���g�̂ݍX�V
			comp->Update(deltaTime);
		}
	}
}

/**
 * �����X�V����
 *
 * �R���|�[�l���g�̕����X�V�������Ăяo��
 */
void GameObject::FixedUpdateComponents(const float deltaTime) {

	for (auto& comp : _components) {
		if (comp->State() == Component::STATE::ACTIVE) { // �A�N�e�B�u��Ԃ̃R���|�[�l���g�̂ݍX�V
			comp->FixedUpdate(deltaTime);
		}
	}
}

/**
 * ���C�g�X�V����
 *
 * �R���|�[�l���g�̃��C�g�X�V�������Ăяo��
 */
void GameObject::LateUpdateConponents(const float deltaTime) {

	for (auto& comp : _components) {
		if (comp->State() == Component::STATE::ACTIVE) { // �A�N�e�B�u��Ԃ̃R���|�[�l���g�̂ݍX�V
			comp->LateUpdate(deltaTime);
		}
	}
}
