#pragma once
#include "Components/CameraComponent.h"

class FollowCameraComponent : public CameraComponent {
public:

	FollowCameraComponent(GameObject* parent);
	~FollowCameraComponent();

	void Update(Frame* frame) override;

	void XOffset(float offset) { _xOffset = offset; };
	void YOffset(float offset) { _yOffset = offset; };
	void AttenRate(float rate) { _attenRate = rate; };
private:
	float _xOffset;
	float _yOffset;
	float _attenRate;
};