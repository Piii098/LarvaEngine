#include "GameObjects/Camera.h"
#include "Components/Camera/FollowCameraComponent.h"
#include "Components/Camera/CameraComponent.h"
#include "Core/Game.h"
#include "GameObjects/Player.h"
#include "Utilities/Input.h"

#pragma region コンストラクタ:デストラクタ

Camera::Camera(Game* game) 
	: GameObject(game){

	_camera = new FollowCameraComponent(this);
	_camera->Target(GetGame()->GetPlayer()->Position());
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

void Camera::UpdateObject(Frame* frame) {
	_camera->Target(GetGame()->GetPlayer()->Position());
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
