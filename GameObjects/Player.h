#pragma once
#include "GameObject.h"


class TestObject;
class RigidbodyComponent;
class SpriteComponent;
class DebugDrawComponent;
class MoveInputComponent;
class TileMapComponent;
class BoxComponent2D;

class Player : public GameObject {
public:
	Player(Game* game);
	~Player() override;

	void InputObject(Input* input) override;
	void UpdateObject(Frame* frame) override;


	void FixCollision(Frame frame);
	
private:
	SpriteComponent* _spriteComp;
	MoveInputComponent* _moveInputComp;
	
	std::vector<TestObject*> _bulletObjects;

	RigidbodyComponent* _rigidbodyComp;
	BoxComponent2D* _boxComp;
	float _jumpForce;
};