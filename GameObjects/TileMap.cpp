#include "GameObjects/TileMap.h"
#include "Components/TileMapSpriteComponent.h"
#include "Components/TileMapComponent.h"
#include "Components/BoxComponent2D.h"
#include "Game.h"
#include "Components/Debug/DebugDrawComponent.h"
#include "GameObjects/GameObject.h"

#pragma region コンストラクタ:デストラクタ

TileMap::TileMap(Game* parent)
	: GameObject(parent){
    Scale(1.f);

	_tileMapComp = new TileMapComponent(this);
	_tileMapComp->SetTileMap("Assets/Test.csv");

	_tileMapSpriComp = new TileMapSpriteComponent(this);
	_tileMapSpriComp->SetTexture("Assets/Tile.png");
	_tileMapSpriComp->SetTileMapComponent(_tileMapComp);

	CreateCollisionBoxes();
}

TileMap::~TileMap() {

}

#pragma endregion

#pragma region パブリック関数

void TileMap::CreateCollisionBoxes() {
	_boxObjects.clear();

	for (int y = 0; y < _tileMapComp->MapHeight(); y++)
	{
		for (int x = 0; x < _tileMapComp->MapWidth(); x++)
		{
			Vector2 tileMin;
			Vector2 tileMax;
			BoxComponent2D* box;
			DebugDrawComponent* debugBox;
			GameObject* obj;
			float halfTile;
			switch (_tileMapComp->Tiles()[y][x]) {
			case 1: case 2: case 6: case 7: case8: case 9:
				halfTile = _tileMapComp->TileSize() * 0.5f;
				tileMin = Vector2(x * _tileMapComp->TileSize() - halfTile,
					(_tileMapComp->MapHeight() - 1 - y) * _tileMapComp->TileSize() - halfTile);
				tileMax = tileMin + Vector2(_tileMapComp->TileSize(), _tileMapComp->TileSize());

				obj = new GameObject(GetGame());
				obj->Tag(GameObject::TAG::GROUND);
				box = new BoxComponent2D(obj, true, false); 

				box->SetObjectBox(AABB2D(tileMin, tileMax));
		
				//debugBox = new DebugDrawComponent(obj, box);

				_boxObjects.push_back(obj);
				break;
			default:
				break;
			}
			
		}
	}
}


#pragma endregion
