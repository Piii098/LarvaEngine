#pragma once
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Audio/SoundEvent.h"

class TestObject;
class RigidbodyComponent;
class SpriteComponent;
class DebugDrawComponent;
class MoveInputComponent;
class TileMapComponent;
class BoxComponent2D;
class AudioComponent;
class LightComponent;

class Player : public GameObject {
public:
	Player(Scene* scene);
	~Player() override;

	void InputObject(Input* input) override;
	void UpdateObject(float deltaTime) override;

private:
	SpriteComponent* _spriteComp;
	MoveInputComponent* _moveInputComp;
	
	std::vector<TestObject*> _bulletObjects;

	RigidbodyComponent* _rigidbodyComp;
	BoxComponent2D* _boxComp;
	AudioComponent* _audioComp;

	LightComponent* _lightComp;

	SoundEvent _footstepEvent;

	float _jumpForce;
};