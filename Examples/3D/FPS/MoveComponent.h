#pragma once
#include "LarvaEngine/Core/Component.h"

class ModeComponent : public Component
{

public:
	ModeComponent(GameObject& gameObject);
	~ModeComponent() override;

	void Update(float deltaTime) override;

private:
	float _strafeSpeed;
};