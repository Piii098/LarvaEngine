#include "GameObjects/TileMapObject.h"
#include "Components/Phys/BoxComponent2D.h"
#include "Components/Draw/DebugDrawComponent.h"
#include "AssetManagers/AssetData/Texture.h"
#include "Renderer/Shader.h"
#include "Core/Game.h"
#include <fstream>
#include <sstream>
#include "AssetManagers/AssetManager.h"
#include "AssetManagers/AssetData/TileMap.h"
#include "Components/TileMapComponent.h"

TileMapObject::TileMapObject(Scene* scene)
    : GameObject(scene){
    Position(Vector2Int(0, -100));
	_tileMapComp = new TileMapComponent(this);
	_tileMapComp->SetTileMap("TileMap", "Tile", 2, 16);

    _tileMapComp->RegisterTileInfo(0, true, Vector3(1, 1, 1), 0., GameObject::TAG::GROUND);

	_tileMapComp->RegisterTileInfo(1, true, Vector3(1, 1, 1), 0., GameObject::TAG::GROUND );
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
