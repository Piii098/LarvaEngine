#pragma once
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/Component.h"

class CameraComponent : public Component {
public:

	CameraComponent(GameObject& parent, int updateLayer = 500);
	~CameraComponent() override;

	void ProcessInput(const InputAction& input) override;
	virtual void Update(float deltaTime) override;

	void Zoom(float zoom) { _zoom = zoom; };
	float Zoom() const { return  _zoom; };
	void Target(const Vector2& target) { _target = target; };
	
	Matrix4 GetViewMatrix() const;

protected:

	Vector2 _target;
	float _zoom;
	Matrix4 _viewMatrix; // ビュー行列
};