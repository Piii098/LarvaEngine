#include "Examples/2D/Scroll/TileMap.h"
#include <SDL3/SDL.h>
#include "LarvaEngine/Components/TileMap/TileMapComponent.h"

Example2DScroll::TileMap::TileMap(Scene& scene)
	: GameObject(scene) {
	Position(Vector3(0, 0, 0));
	auto& tileMapComp = CreateComponent<TileMapComponent>(16, "TileTexture_0");
	tileMapComp.SetTileMap("TileMap_0");
	tileMapComp.SetCollision(0, true);
	tileMapComp.SetCollision(1, true);
	_tileMapComp = &tileMapComp;
}

Example2DScroll::TileMap::~TileMap() {

}

void Example2DScroll::TileMap::SetOnTile(int tileID, GameObject* obj) {
	Vector2Int pos = _tileMapComp->GetTilePosition(tileID);
	obj->Position(Vector3(pos.x, pos.y, 0));
	SDL_Log("SetOnTile: %d, %d", pos.x, pos.y);
}