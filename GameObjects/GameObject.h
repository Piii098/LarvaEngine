#pragma once
#include <vector>
#include "Math.h"

class Input;
class Frame;
class Component;
class Game;

class GameObject {
public:

	enum class STATE {
		ACTIVE,
		PAUSED,
		DEAD
	};

	enum class TAG {
		NONE,
		PLAYER,
		ENEMY,
		ITEM,
		GROUND,
		OBSTACLE
	};

	GameObject(Game* game);
	virtual ~GameObject();

	/*����*/

	void ProcessInput(Input* input);
	virtual void InputObject(Input* input);

	/*�X�V*/

	void Update(Frame* frame); // Game�N���X�Ŏ��s�����X�V
	void UpdateComponents(Frame* frame); // �SComponent�̍X�V
	virtual void UpdateObject(Frame* frame); // �p����I�u�W�F�N�g�̃��j�[�N�ȍX�V
	virtual void PhysUpdate(Frame* frame);

	void ComputeWorldTransform();

	/*�R���|�[�l���g����*/

	void AddComponent(Component* component); //�R���|�[�l���g������(�����ɍ��킹)
	void RemoveComponent(Component* component);  //�R���|�[�l���g�����O��

	/*�Q�b�^�[�Z�b�^�[*/

	Game* GetGame() { return _game; };
	Vector2 GetForward() const { return Vector2(Math::Cos(_rotation), Math::Sin(_rotation)); };

	const Matrix4& WorldTransform() const { return _worldTransform; };
	const Vector2& Position() const { return _position; };
	TAG Tag() const { return _tag; };
	STATE State() const { return _state; };
	float Scale() const { return _scale; };
	float Rotation() const { return _rotation; };
	void Position(const Vector2 position) { _position = position; _recomputeWorldTransform = true; };
	void PixelPosition(Vector2 position) { _position = Vector2(static_cast<int>(position.x), static_cast<int>(position.y));}
	void Scale(float scale) { _scale = scale; _recomputeWorldTransform = true;};
	void Rotation(float rotation) { _rotation = rotation; _recomputeWorldTransform = true;};
	void Tag(TAG tag) { _tag = tag; };
	void State(STATE state) { _state = state; };
private:

	STATE _state;
	TAG _tag;
	Game* _game;

	/*�g�����X�t�H�[���ϐ�*/

	Matrix4 _worldTransform;
	bool _recomputeWorldTransform;

	Vector2 _position; // �ʒu
	float _scale; // �X�P�[��
	float _rotation; // ��]

	std::vector<Component*> _components; //�������Ă���R���|�[�l���g�̏W�܂�

};