#include "GameObjects/Camera.h"
#include "Components/FollowCameraComponent.h"
#include "Game.h"
#include "GameObjects/Player.h"
#include "Utilities/Input.h"

#pragma region コンストラクタ:デストラクタ

Camera::Camera(Game* game) 
	: GameObject(game){

	_camera = new FollowCameraComponent(this);
	_camera->Target(GetGame()->GetPlayer()->Position());
	_zoom = 2.f;
	_camera->Zoom(_zoom);
	_camera->YOffset(0.f);
	_camera->AttenRate(7.f);
}

Camera::~Camera() {

}

#pragma endregion


#pragma region パブリック関数

void Camera::InputObject(Input* input) {
	if (input->IsInputDown(InputMap::INPUT_BUP)) {
		_zoom += 0.5f;
	}
	if (input->IsInputDown(InputMap::INPUT_BDOWN)) {
		_zoom -= 0.5f;
	}
	_camera->Zoom(_zoom);
}

void Camera::UpdateObject(Frame* frame) {
	_camera->Target(GetGame()->GetPlayer()->Position());
}

Matrix4 Camera::GetViewMatrix() const {
	return _camera->GetViewMatrix();
}

#pragma endregion
