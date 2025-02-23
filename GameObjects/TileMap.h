#pragma once
#include <vector>
#include "GameObjects/GameObject.h"

class TileMapSpriteComponent;
class TileMapComponent;
class BoxComponent2D;
class DebugDrawComponent;
class TileMap : public GameObject {
public:
	TileMap(Game* parent);
	~TileMap() override;

	void CreateCollisionBoxes();

	TileMapComponent* GetTileMapComponent() { return _tileMapComp; };

private:

	TileMapSpriteComponent* _tileMapSpriComp;
	TileMapComponent* _tileMapComp;

	std::vector<GameObject*> _boxObjects;
};