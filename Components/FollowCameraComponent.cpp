#include "Components/FollowCameraComponent.h"
#include "GameObjects/GameObject.h"
#include "Utilities/Frame.h"

#pragma region �V���O���g��()

#pragma endregion


#pragma region �R���X�g���N�^:�f�X�g���N�^

FollowCameraComponent::FollowCameraComponent(GameObject* parent)
    : CameraComponent(parent)
    , _xOffset(0.f)
    , _yOffset(0.f)
    , _attenRate(3.f){

}

FollowCameraComponent::~FollowCameraComponent() {

}
#pragma endregion

#pragma region �p�u���b�N�֐�

void FollowCameraComponent::Update(Frame* frame) {
    Vector2 pos = _target + Vector2(_xOffset, _yOffset);
    _parent->Position(Vector2::Lerp(_parent->Position(), pos, frame->DeltaTime() * _attenRate));

}

#pragma endregion

#pragma region �v���C�x�[�g�֐�


#pragma endregion

