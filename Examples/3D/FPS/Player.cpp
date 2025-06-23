#include "Examples/3D/FPS/Player.h"
#include "LarvaEngine/Components/Draw/ModelComponent.h"
#include "LarvaEngine/Components/Physics/Rigidbody3DComponent.h"
#include "Examples/3D/FPS/FPSCameraComponent.h"
#include "LarvaEngine/Input/InputAction.h"
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/Scene.h"

Example3DFPS::Player::Player(Scene& scene)
	: Camera(scene)
{
	Position(Vector3(0, 0, 100));
	//ModelComponent& modelComp = CreateComponent<ModelComponent>();
	CreateComponent<Rigidbody3DComponent>();
	CreateComponent<FPSCameraComponent>();
	SetCameraComponent(GetComponent<FPSCameraComponent>());
	//ModelComponent& modelComp = CreateComponent<ModelComponent>();
	//modelComp.SetModel("table");

	GameObject& gun = GetScene().CreateChildObject<GameObject>(this);
	ModelComponent& modelComp = gun.CreateComponent<ModelComponent>();
	modelComp.SetModel("Gun");
	gun.Scale(0.20f);
	gun.Position(Vector3(-20, 40, -10));
	gun.Rotation(Quaternion(Vector3::UnitZ, Math::ToRadians(-110.f)));

	_rigidbodyComp = GetComponent<Rigidbody3DComponent>();
}

Example3DFPS::Player::~Player()
{
	// プレイヤーのクリーンアップ処理
}

void Example3DFPS::Player::UpdateObject(float deltaTime)
{
	// SDL_Log("Position: %f, %f, %f", Position().x, Position().y, Position().z);
}

void Example3DFPS::Player::InputObject(const InputAction& input)
{
	_direction = Vector2::Zero;
	float straife = input.GetActionValue("Horizontal");
	float forward = input.GetActionValue("Vertical");

	_direction = Vector2(forward, straife);
	
	// nomalize
	if (_direction.Length() > 1.0f) {
		_direction.Normalize();
	}

	input.SetRelativeMouseMode(true);
	Vector2 mouse = input.GetMousePosition();

	const float mouseSensitivity = 0.01f; // ここで感度を調整（0.05〜0.2ぐらいで好みに合わせて）

	const float maxYawSpeed = Math::Pi * 8.f;
	float yawSpeed = 0.0f;
	if (mouse.x != 0) {
		yawSpeed = mouse.x * maxYawSpeed * mouseSensitivity * 0.01;
	}

	Quaternion q(Quaternion::Concatenate(Rotation(), Quaternion(Vector3::UnitZ, yawSpeed)));

	Rotation(q);

	const float maxPitchSpeed = Math::Pi * 8.f;
	float pitchSpeed = 0.0f;
	if (mouse.y != 0) {
		pitchSpeed = mouse.y * maxPitchSpeed * mouseSensitivity;
	}
	GetComponent<FPSCameraComponent>()->PitchSpeed(-pitchSpeed);

	//SDL_Log("PitchSpeed: %f", pitchSpeed);

}

void Example3DFPS::Player::FixedUpdateObject(float deltaTime)
{
	Vector2 newVec = _direction * 40000 * deltaTime;

	_rigidbodyComp->Move(newVec.x, newVec.y);
	
}