#pragma once
#include "Components/Component.h"
#include "Utilities/Collision2D.h"

class CapsuleComponent : public Component {
public:
	CapsuleComponent();
	~CapsuleComponent();



private:
	Capsule2D _objectCapsule;
	Capsule2D _worldCapsule;

	bool _isCollision;
	bool _isDynamic;
};