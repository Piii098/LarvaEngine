#pragma once
#include "LarvaEngine/Core/GameObject.h"

class SpriteComponent;
class RigidbodyComponent;
class MoveInputComponent;
class BoxComponent2D;

namespace Example2DScroll {

	class Player : public GameObject {
	public:

		Player(Scene& scene);
		~Player() override;

		void UpdateObject(float deltaTime) override;


	private:

	};
}