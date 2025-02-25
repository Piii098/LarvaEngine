#pragma once
#include "Math.h"
#include "Components/Component.h"

class CameraComponent : public Component {
public:

	CameraComponent(GameObject* parent, int updateLayer = 500);
	~CameraComponent() override;

	void ProcessInput(Input* input) override;
	void Update(Frame* frame) override;

	void Zoom(float zoom) { _zoom = zoom; };
	float Zoom() { return  _zoom; };
	void Target(const Vector2& target) { _target = target; };
	
	Matrix4 GetViewMatrix() const;

protected:

	Vector2 _target;
	float _zoom;

};