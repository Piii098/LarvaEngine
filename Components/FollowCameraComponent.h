#pragma once
#include "Components/CameraComponent.h"

class FollowCameraComponent : public CameraComponent {
public:

	FollowCameraComponent(GameObject* parent);
	~FollowCameraComponent() override;

	void Update(Frame* frame) override;

	void XOffset(float offset) { _xOffset = offset; };
	void YOffset(float offset) { _yOffset = offset; };
	void AttenRate(float rate) { _attenRate = rate; };
private:
	int _xOffset;
	int _yOffset;
	float _attenRate;
};