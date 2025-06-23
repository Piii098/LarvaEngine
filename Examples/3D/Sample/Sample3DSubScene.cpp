#include "Examples/3D/Sample/Sample3DSubScene.h"
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/GameObject.h"

Sample3D::Sample3DSubScene::Sample3DSubScene(MainScene& scene, GameObject& cube)
	: SubScene(scene)
	, _cube(cube)
{
}

Sample3D::Sample3DSubScene::~Sample3DSubScene()
{
}

void Sample3D::Sample3DSubScene::UpdateScene(float deltaTime) {
    // ��]���Ɖ�]���x���`
    //Vector3 rotationAxis = Vector3::UnitY; // Z������̉�]
   // float rotationSpeed = Math::ToRadians(45.0f); // 45�x/�b

    // 1�t���[��������̉�]�p���v�Z
   // float rotationAngle = rotationSpeed * deltaTime;

    // �V������]���쐬
    //Quaternion deltaRotation(rotationAxis, rotationAngle);

	//_cube.Rotation(Quaternion::Concatenate(_cube.Rotation(), deltaRotation));

	// �ʒu���X�V
	Vector3 position = _cube.Position();
	position.y += 100.0f * deltaTime; // X��������1���j�b�g/�b�ňړ�
	_cube.Position(position);

}