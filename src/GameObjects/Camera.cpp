#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Components/Camera/FollowCameraComponent.h"
#include "LarvaEngine/Components/Camera/CameraComponent.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/GameObjects/Player.h"
#include "LarvaEngine/Core/Events/Input.h"

#pragma region コンストラクタ:デストラクタ

Camera::Camera(Scene* scene)
	: GameObject(scene){
	Position(Vector2Int(800, 500));
	_camera = new FollowCameraComponent(this);
	//_camera->Target(GetGame()->GetPlayer()->Position());
	_zoom = 2.f;
	_camera->Zoom(_zoom);
	_camera->YOffset(35.f);
	_camera->AttenRate(4);
}

Camera::~Camera() {
	delete _camera;
	_camera = nullptr;
}

#pragma endregion


#pragma region パブリック関数

void Camera::InputObject(Input* input) {
	if (input->IsInputDown(InputMap::INPUT_BUP)) {
		_zoom += 1;
	}
	if (input->IsInputDown(InputMap::INPUT_BDOWN)) {
		_zoom -= 1;
	}
	_camera->Zoom(_zoom);
}

void Camera::UpdateObject(float deltaTime) {
	_camera->Target(_target->Position());
}

Matrix4 Camera::GetViewMatrix() const {
	return _camera->GetViewMatrix();
}

float Camera::Zoom() const {
	return _camera->Zoom();
}

Vector2 Camera::SubPixelOffset() const {
	return _camera->SubPixelOffset();
}

#pragma endregion
