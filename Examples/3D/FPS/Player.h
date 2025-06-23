#pragma once
#include "LarvaEngine/GameObjects/Camera.h"

class Rigidbody3DComponent;

namespace Example3DFPS
{
	class Player : public Camera
	{
	public:
		Player(Scene& scene);
		~Player() override;

		void UpdateObject(float deltaTime) override;
		void InputObject(const InputAction& input) override;
		void FixedUpdateObject(float deltaTime) override;

	private:
		Vector2 _direction;
		Rigidbody3DComponent* _rigidbodyComp;
	};
}