#include "Examples/3D/FPS/FPSCameraComponent.h"
#include "LarvaEngine/Core/GameObject.h"

Example3DFPS::FPSCameraComponent::FPSCameraComponent(GameObject& gameObject)
	: CameraComponent(gameObject)
	, _pitchSpeed(0.0f)
	, _maxPitch(Math::Pi / 3.0f)
	, _pitch(0.0f)
{
}

Example3DFPS::FPSCameraComponent::~FPSCameraComponent()
{
	// カメラのクリーンアップ処理
}

void Example3DFPS::FPSCameraComponent::Update(float deltaTime)
{
	Vector3 pos = GetParent().Position();

	_pitch += _pitchSpeed * deltaTime;
	_pitch = Math::Clamp(_pitch, -_maxPitch, _maxPitch);
	Quaternion q(_parent.GetRight(), _pitch);

	Vector3 viewForward = Vector3::Transform(_parent.GetForward(), q);


	Vector3 target = pos + viewForward * 100.f;
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);
	_viewMatrix = Matrix4::CreateLookAt(pos, target, up);

}
