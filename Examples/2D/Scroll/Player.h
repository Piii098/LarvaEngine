#pragma once
#include "LarvaEngine/Core/GameObject.h"

class SpriteComponent;
class Rigidbody2DComponent;
class MoveInputComponent;
class Box2DComponent;

namespace Example2DScroll {

	class Player : public GameObject {
	public:

		Player(Scene& scene);
		~Player() override;

		void UpdateObject(float deltaTime) override;


	private:

	};
}