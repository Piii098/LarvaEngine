#pragma once
#include "GameObject.h"

class DynamicComponent;
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
	void PlacePlayerAtTile(int tileX, int tileY, TileMapComponent* tileMap);
	void PlacePlayerAtSpawn(TileMapComponent* tileMap);
private:
	SpriteComponent* _spriteComp;
	MoveInputComponent* _moveInputComp;
	
	DynamicComponent* _dynamicComp;
	float _jumpForce;
};