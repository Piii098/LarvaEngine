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

	/*入力*/

	void ProcessInput(Input* input);
	virtual void InputObject(Input* input);

	/*更新*/

	void Update(Frame* frame); // Gameクラスで実行される更新
	void UpdateComponents(Frame* frame); // 全Componentの更新
	virtual void UpdateObject(Frame* frame); // 継承先オブジェクトのユニークな更新
	virtual void PhysUpdate(Frame* frame);

	void ComputeWorldTransform();

	/*コンポーネント処理*/

	void AddComponent(Component* component); //コンポーネントをつける(順序に合わせ)
	void RemoveComponent(Component* component);  //コンポーネントを取り外す

	/*ゲッターセッター*/

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

	/*トランスフォーム変数*/

	Matrix4 _worldTransform;
	bool _recomputeWorldTransform;

	Vector2 _position; // 位置
	float _scale; // スケール
	float _rotation; // 回転

	std::vector<Component*> _components; //所持しているコンポーネントの集まり

};