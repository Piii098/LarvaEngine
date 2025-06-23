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
    // 回転軸と回転速度を定義
    //Vector3 rotationAxis = Vector3::UnitY; // Z軸周りの回転
   // float rotationSpeed = Math::ToRadians(45.0f); // 45度/秒

    // 1フレームあたりの回転角を計算
   // float rotationAngle = rotationSpeed * deltaTime;

    // 新しい回転を作成
    //Quaternion deltaRotation(rotationAxis, rotationAngle);

	//_cube.Rotation(Quaternion::Concatenate(_cube.Rotation(), deltaRotation));

	// 位置を更新
	Vector3 position = _cube.Position();
	position.y += 100.0f * deltaTime; // X軸方向に1ユニット/秒で移動
	_cube.Position(position);

}