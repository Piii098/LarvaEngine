#pragma once

class GameObject;
class Input;
class Frame;
class Shader;

class Component {
public:

	enum class STATE {
		ACTIVE,
		INACTIVE,
		DEAD
	};

	Component(GameObject* parent, int updateLayer = 100);
	virtual ~Component();

	virtual void ProcessInput(Input* input) {};
	virtual void Update(float deltaTime) {};
	virtual void PhysUpdate(float deltaTime) {};
	virtual void OnUpdateWorldTransform() {};
	
	/*ゲッターセッター*/

	int GetBufferLayer() const { return _bufferLayer; };
	void SetBufferLayer(int bufferLayer) { _bufferLayer = bufferLayer; };
	int GetRenderType() const { return _renderType; };
	GameObject* GetParent() const { return _parent; };
	int UpdateLayer() const { return _updateLayer; };

	STATE GetState() const { return _state; };
	void SetState(STATE state) { _state = state; };

protected:
	STATE _state;
	GameObject* _parent;
	int _updateLayer;
	int _bufferLayer;
	int _renderType;
};