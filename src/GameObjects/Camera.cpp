#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Components/Camera/FollowCameraComponent.h"
#include "LarvaEngine/Components/Camera/CameraComponent.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/GameObjects/Player.h"
#include "LarvaEngine/Input/InputAction.h"

#pragma region コンストラクタ:デストラクタ

Camera::Camera(Scene& scene)
	: GameObject(scene)
	, _camera(CreateComponent<FollowCameraComponent>()){
	Position(Vector2Int(800, 500));
	_zoom = 2.f;
	_camera.Zoom(_zoom);
	_camera.YOffset(35.f);
	_camera.AttenRate(4);
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
	_camera.Zoom(_zoom);
}

void Camera::UpdateObject(float deltaTime) {
	_camera.Target(_target->Position());
}

Matrix4 Camera::GetViewMatrix() const {
	return _camera.GetViewMatrix();
}

float Camera::Zoom() const {
	return _camera.Zoom();
}

Vector2 Camera::SubPixelOffset() const {
	return _camera.SubPixelOffset();
}

#pragma endregion
