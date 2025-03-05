#include "Components/Camera/CameraComponent.h"
#include "GameObjects/GameObject.h"
#include "Utilities/Input.h"

#pragma region シングルトン()

#pragma endregion


#pragma region コンストラクタ:デストラクタ

CameraComponent::CameraComponent(GameObject* parent, int updateLayer)
    :Component(parent, updateLayer)
    , _target(Vector2Int::Zero)
    , _zoom(1.f){

}

CameraComponent::~CameraComponent() {

}
#pragma endregion

#pragma region パブリック関数

void CameraComponent::ProcessInput(Input* input) {

}

void CameraComponent::Update(float deltaTime) {

}

Matrix4 CameraComponent::GetViewMatrix() const{
    Matrix4 translation = Matrix4::CreateTranslation(Vector3(-_parent->Position().x, -_parent->Position().y, 0.0f));
    Matrix4 scale = Matrix4::CreateScale(1, 1, 1);
    return translation * scale;
}



#pragma endregion

#pragma region プライベート関数


#pragma endregion

