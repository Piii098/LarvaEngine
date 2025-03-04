#include "GameObjects/TileMapObject.h"
#include "Components/BoxComponent2D.h"
#include "Components/Debug/DebugDrawComponent.h"
#include "Utilities/Texture.h"
#include "Utilities/Shader.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include "AssetManagers/AssetManager.h"
#include "AssetManagers/TileMap.h"
#include "Components/TileMapComponent.h"

TileMapObject::TileMapObject(Game* game)
    : GameObject(game){
	_tileMapComp = new TileMapComponent(this);
	_tileMapComp->SetTileMap("TileMap", "Tile", 24, 32);

	_tileMapComp->RegisterTileInfo(1, true, Vector3(1, 1, 1), 0., GameObject::TAG::GROUND );
    _tileMapComp->RegisterTileInfo(2, true, Vector3(1, 1, 1), 0., GameObject::TAG::GROUND);
	_tileMapComp->RegisterTileInfo(6, true, Vector3(1, 1, 1), 0., GameObject::TAG::GROUND);
	_tileMapComp->RegisterTileInfo(7, true, Vector3(1, 1, 1), 0., GameObject::TAG::GROUND);
    _tileMapComp->RegisterTileInfo(8, true, Vector3(1, 1, 1), 0., GameObject::TAG::GROUND);
    _tileMapComp->RegisterTileInfo(9, true, Vector3(1, 1, 1), 0., GameObject::TAG::GROUND);
    _tileMapComp->CreateTiles();
}

TileMapObject::~TileMapObject() {
    
}


#pragma region パブリック関数

void TileMapObject::SetOnTile(GameObject* object, int tileId) {
    for (auto tile : GetChildren()) {
        Tile* tileObject = static_cast<Tile*>(tile);
        if (tileObject && tileObject->TileId() == tileId) {
            object->Position(tileObject->Position());
            _tileMapComp->DestroyTile(tileId);
            break;
        }
    }
}

#pragma endregion
