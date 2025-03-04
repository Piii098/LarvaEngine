#pragma once

class GameObject;
class Input;
class Frame;

class Component {
public:
	Component(GameObject* parent, int updateLayer = 100);
	virtual ~Component();

	virtual void ProcessInput(Input* input); 
	virtual void Update(Frame* frame);
	virtual void PhysUpdate(float deltaTime);
	virtual void OnUpdateWorldTransform() {};
	
	/*ゲッターセッター*/

	GameObject* GetParent() const { return _parent; };
	int UpdateLayer() const { return _updateLayer; };
protected:
	GameObject* _parent;
	int _updateLayer;
};