#pragma once
#include "LarvaEngine/Components/Camera/CameraComponent.h"

namespace Example3DFPS
{

	class FPSCameraComponent : public CameraComponent
	{
	public:
		FPSCameraComponent(GameObject& gameObject);
		~FPSCameraComponent() override;

		void Update(float deltaTime) override;

		void PitchSpeed(float speed) { _pitchSpeed = speed; }
		void MaxPitch(float max) { _maxPitch = max; }
		void Pitch(float pitch) { _pitch = pitch; }

		float PitchSpeed() const { return _pitchSpeed; }
		float MaxPitch() const { return _maxPitch; }
		float Pitch() const { return _pitch; }
	private:
		float _pitchSpeed;
		float _maxPitch;
		float _pitch;
	};

}
