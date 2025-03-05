#pragma once
#include "GameObjects/GameObject.h"

class FollowCameraComponent;
class GameObject;

class Camera : public GameObject {
public:
	Camera(Scene* scene);
	~Camera() override;

	void InputObject(Input* input) override;
	void UpdateObject(float deltaTime) override;

	Matrix4 GetViewMatrix() const;
	float Zoom() const;
	Vector2 SubPixelOffset() const;

	void Target(GameObject* target) { _target = target; }

private:
	FollowCameraComponent* _camera;
	GameObject* _target;
	float _zoom;
};