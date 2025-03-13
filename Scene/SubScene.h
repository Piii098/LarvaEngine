#pragma once

class MainScene;
class Input;

class SubScene {
public:
	SubScene(MainScene* parent) : _parent(parent) {};
	virtual ~SubScene() {};

	virtual void Initialize() {};

	virtual void InputScene(Input* input) {};
	virtual void UpdateScene(float deltaTime) {};

	virtual void Shutdown() {};

protected:
	MainScene* _parent;
};