#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Components/Camera/FollowCameraComponent.h"
#include "LarvaEngine/Components/Camera/CameraComponent.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/GameObjects/Player.h"
#include "LarvaEngine/Input/InputAction.h"

#pragma region コンストラクタ:デストラクタ

Camera::Camera(Scene& scene)
	: GameObject(scene)
	, _cameraComp(&CreateComponent<CameraComponent>()){
	Position(Vector2Int(0, 0));
	_zoom = 2.f;
	_cameraComp->Zoom(_zoom);
	
}

Camera::~Camera() {

}

#pragma endregion


#pragma region パブリック関数

void Camera::InputObject(const InputAction& input) {
	if (input.IsKeyDown(SDL_SCANCODE_UP)) {
		_zoom += 1;
	}
	if (input.IsKeyDown(SDL_SCANCODE_DOWN)) {
		_zoom -= 1;
	}
	_cameraComp->Zoom(_zoom);
}

void Camera::UpdateObject(float deltaTime) {

}

Matrix4 Camera::GetViewMatrix() const {
	return _cameraComp->GetViewMatrix();
}

float Camera::Zoom() const {
	return _cameraComp->Zoom();
}

Vector2 Camera::SubPixelOffset() const {
	return _cameraComp->SubPixelOffset();
}

#pragma endregion
