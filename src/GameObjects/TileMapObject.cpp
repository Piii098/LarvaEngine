#include "LarvaEngine/GameObjects/TileMapObject.h"
#include "LarvaEngine/Components/Physics/BoxComponent2D.h"
#include "LarvaEngine/Components/Draw/DebugDrawComponent.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Core/Game.h"
#include <fstream>
#include <sstream>
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/TileMap.h"
#include "LarvaEngine/Components/TileMapComponent.h"

TileMapObject::TileMapObject(Scene& scene)
    : GameObject(scene){
    Position(Vector2Int(0, -100));
    TileMapComponent* tileMapComp = CreateComponent<TileMapComponent>();

    tileMapComp->RegisterTileInfo(0, true, Vector3(1, 1, 1), 0., GameObject::TAG::GROUND);

    tileMapComp->RegisterTileInfo(1, true, Vector3(1, 1, 1), 0., GameObject::TAG::GROUND );
    tileMapComp->CreateTiles();
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
