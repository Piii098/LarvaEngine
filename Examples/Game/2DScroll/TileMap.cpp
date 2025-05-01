#include "Examples/Game/2DScroll/TileMap.h"
#include "LarvaEngine/Components/TileMap/TileMapComponent.h"

Example2DScroll::TileMap::TileMap(Scene& scene)
	: GameObject(scene) {
	Position(Vector2Int(0, 0));
	auto& tileMap = CreateComponent<TileMapComponent>("TileMap_0", 16, "TileTexture_0");

}

Example2DScroll::TileMap::~TileMap() {
	// TileMapのクリーンアップ処理
}

