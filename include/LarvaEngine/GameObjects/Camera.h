#pragma once
#include "LarvaEngine/Core/GameObject.h"

class CameraComponent;
class GameObject;

class Camera : public GameObject {
public:
	Camera(Scene& scene);
	~Camera() override;

	void InputObject(const InputAction& input) override;
	void UpdateObject(float deltaTime) override;

	Matrix4 GetViewMatrix() const;
	float Zoom() const;
	Vector2 SubPixelOffset() const;


private:
	CameraComponent* _cameraComp;
	float _zoom;
};