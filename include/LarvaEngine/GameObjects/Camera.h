#pragma once
#include "LarvaEngine/Core/GameObject.h"

class CameraComponent;
class GameObject;

class Camera : public GameObject {
public:
	Camera(Scene& scene);
	~Camera() override;

	virtual void InputObject(const InputAction& input) override;
	virtual void UpdateObject(float deltaTime) override;

	Matrix4 GetViewMatrix() const;
	float Zoom() const;

	void SetCameraComponent(CameraComponent* cameraComp);

private:
	CameraComponent* _cameraComp;
	float _zoom;
};