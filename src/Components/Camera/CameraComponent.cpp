#include "LarvaEngine/Components/Camera/CameraComponent.h"
#include "LarvaEngine/Core/GameObject.h"

#pragma region シングルトン()

#pragma endregion


#pragma region コンストラクタ:デストラクタ

CameraComponent::CameraComponent(GameObject& parent, int updateLayer)
    :Component(parent, updateLayer)
    , _target(Vector2Int::Zero)
    , _zoom(1.f){

}

CameraComponent::~CameraComponent() {

}
#pragma endregion

#pragma region パブリック関数

void CameraComponent::ProcessInput(const InputAction& input) {

}

void CameraComponent::Update(float deltaTime) {
    Matrix4 translation = Matrix4::CreateTranslation(Vector3(-_parent.Position().x, -_parent.Position().y, 0.0f));
    Matrix4 scale = Matrix4::CreateScale(1 * _zoom, 1 * _zoom, 1 * _zoom);
	_viewMatrix = translation * scale;
}

Matrix4 CameraComponent::GetViewMatrix() const{
    return _viewMatrix;
}



#pragma endregion

#pragma region プライベート関数


#pragma endregion

