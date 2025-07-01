#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Components/Camera/FollowCameraComponent.h"
#include "LarvaEngine/Components/Camera/CameraComponent.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Input/InputAction.h"

#pragma region コンストラクタ:デストラクタ

Camera::Camera(Scene& scene)
	: GameObject(scene)
	, _cameraComp(nullptr){
	_zoom = 1.f;
	CreateComponent<CameraComponent>();
	_cameraComp = GetComponent<CameraComponent>();
	Position(Vector3(0, 0, 0));
}

Camera::~Camera() {

}

#pragma endregion


#pragma region パブリック関数

void Camera::InputObject(const InputAction& input) {
	if (input.IsKeyDown(SDL_SCANCODE_1)) {
		_zoom += 1;
	}
	if (input.IsKeyDown(SDL_SCANCODE_2)) {
		_zoom -= 1;
	}
	_cameraComp->Zoom(_zoom);
}

void Camera::UpdateObject(float deltaTime) {

}

Matrix4 Camera::GetViewMatrix() const {
	if (_cameraComp == nullptr) {
		return Matrix4::Identity;
	}

	return _cameraComp->GetViewMatrix();
}

void Camera::SetCameraComponent(CameraComponent* cameraComp) {
	DestroyComponent(_cameraComp);
	_cameraComp = cameraComp;
}

float Camera::Zoom() const {
	if (_cameraComp == nullptr) {
		return 1.0f;
	}
	return _cameraComp->Zoom();
}


#pragma endregion
