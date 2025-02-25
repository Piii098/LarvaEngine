#include "GameObjects/Player.h"
#include "Components/SpriteComponent.h"
#include "Components/MoveInputComponent.h"
#include "Components/RigidbodyComponent.h"
#include "Game.h"
#include "Utilities/Input.h"
#include <SDL3/SDL.h>
#include "Components/TileMapComponent.h"
#include "Utilities/Collision2D.h"
#include "Components/BoxComponent2D.h"
#include "Components/Debug/DebugDrawComponent.h"
#include <iostream>
#include "Utilities/Frame.h"
#include "PhysWorld2D.h"
#include "GameObjects/TestObject.h"

#pragma region コンストラクタ:デストラクタ

Player::Player(Game* game) 
	: GameObject(game) {
	Tag(TAG::PLAYER);
	Scale(1.f);
	_spriteComp = new SpriteComponent(this);
	_spriteComp->SetTexture("Assets/AKAGE.png");

	_jumpForce = 700.f;

	_dynamicComp = new DynamicComponent(this, true);
	AABB2D myBox(Vector2(-6.0f, -16.f), Vector2(6.0f, 13.0f));
	_dynamicComp->boxComp->SetObjectBox(myBox);
	_dynamicComp->rigidbodyComp->Velocity(Vector2::Zero);
	_dynamicComp->rigidbodyComp->Mass(50.f);
	_dynamicComp->rigidbodyComp->IsGravity(true);
	_dynamicComp->rigidbodyComp->Drag(0.f);

	_moveInputComp = new MoveInputComponent(this, _dynamicComp->rigidbodyComp);
	_moveInputComp->MoveSpeed(100.f);
	_moveInputComp->JumpForce(500000.f);

	//new DebugDrawComponent(this, _dynamicComp->boxComp, true);
}

Player::~Player() {
	delete _spriteComp;
	_spriteComp = nullptr;
	delete _moveInputComp;
	_moveInputComp = nullptr;
	delete _dynamicComp;
	_dynamicComp = nullptr;

}
#pragma endregion


#pragma region パブリック関数

void Player::InputObject(Input* input) {
	if (input->IsInputDown(InputMap::INPUT_BRIGHT)) {
		TestObject* bullet = new TestObject(GetGame());
		bullet->Position(Position());
	}
}

void Player::UpdateObject(Frame* frame) {


	if (_moveInputComp->Direction() < 0.f) { // 左向きに移動したら
		_spriteComp->FlipX(false);
	}
	if (_moveInputComp->Direction() > 0.f) {
		_spriteComp->FlipX(true);
	}
	
}

void Player::FixCollision(Frame frame) {
	
}


void Player::PlacePlayerAtTile(int tileX, int tileY, TileMapComponent* tileMapComp) {
	float tileSize = tileMapComp->TileSize();
	int mapHeight = tileMapComp->MapHeight();
	float worldX = tileX * tileSize;
	float worldY = (mapHeight - 1 - tileY) * tileSize;

	Position(Vector2(worldX, worldY));
}

void Player::PlacePlayerAtSpawn(TileMapComponent* tileMapComp)
{
	const auto& tiles = tileMapComp->Tiles();
	int mapHeight = tileMapComp->MapHeight();
	int mapWidth = tileMapComp->MapWidth();
	float tileSize = tileMapComp->TileSize();

	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			if (tiles[y][x] == -2)
			{
				float worldX = x * tileSize;
				float worldY = (mapHeight - 1 - y) * tileSize;

				Position(Vector2(worldX, worldY));

				// 必要なら、スポーンタイルを通常のタイルに置き換える処理も
				// tileMap->Tiles()[y][x] = defaultTileValue;

				return;
			}
		}
	}

	SDL_Log("Error: Spawn tile not found in tile map!");
}

#pragma endregion

