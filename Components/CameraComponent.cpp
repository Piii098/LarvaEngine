#include "Components/CameraComponent.h"
#include "GameObjects/GameObject.h"
#include "Utilities/Input.h"

#pragma region �V���O���g��()

#pragma endregion


#pragma region �R���X�g���N�^:�f�X�g���N�^

CameraComponent::CameraComponent(GameObject* parent, int updateLayer)
    :Component(parent, updateLayer)
    , _target(Vector2::Zero)
    , _zoom(1.f){

}

CameraComponent::~CameraComponent() {

}
#pragma endregion

#pragma region �p�u���b�N�֐�

void CameraComponent::ProcessInput(Input* input) {

}

void CameraComponent::Update(Frame* frame) {

}

Matrix4 CameraComponent::GetViewMatrix() const{
    Matrix4 translation = Matrix4::CreateTranslation(Vector3(-_parent->Position().x, -_parent->Position().y, 0.0f));
    Matrix4 scale = Matrix4::CreateScale(_zoom, _zoom, 1.f);
    return translation * scale;
}

#pragma endregion

#pragma region �v���C�x�[�g�֐�


#pragma endregion

