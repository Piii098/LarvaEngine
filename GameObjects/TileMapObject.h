#pragma once
#include "GameObjects/GameObject.h"
#include <vector>
#include <string>

class Game;
class BoxComponent2D;
class TileMapComponent;
class Texture;
class Shader;
class Tile;
class TileMap;
template <typename T>
class AssetManager;

class TileMapObject : public GameObject {
public:

    TileMapObject(Scene* scene);
    ~TileMapObject() override;
    void SetOnTile(GameObject* object, int tileId);
	void RemoveTile(int tileId);
private:
	TileMapComponent* _tileMapComp;
};