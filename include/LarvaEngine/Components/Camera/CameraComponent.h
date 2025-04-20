#pragma once
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/Component.h"

class CameraComponent : public Component {
public:

	CameraComponent(GameObject& parent, int updateLayer = 500);
	~CameraComponent() override;

	void ProcessInput(const InputAction& input) override;
	void Update(float deltaTime) override;

	void Zoom(float zoom) { _zoom = zoom; };
	float Zoom() const { return  _zoom; };
	void Target(const Vector2Int& target) { _target = target; };
	
	Vector2 SubPixelOffset() const { return _subPixelOffset; };	
	Matrix4 GetViewMatrix() const;

protected:

	Vector2Int _target;
	float _zoom;
	Vector2 _subPixelOffset; // サブピクセルオフセット保持用
};