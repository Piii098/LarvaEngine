#include "Scene/Scenes/TestScene.h"
#include "GameObjects/Player.h"
#include "GameObjects/TileMapObject.h"
#include "GameObjects/Background.h"
#include "GameObjects/Camera.h"
#include "Scene/SceneManager.h"
#include "Core/Game.h"
#include "AssetManagers/AssetManager.h"
#include "AssetManagers/AssetData/Texture.h"
#include "AssetManagers/AssetData/TileMap.h"

#pragma region コンストラクタ:デストラクタ

TestScene::TestScene(SceneManager* manager) 
	: Scene(manager) {

}

TestScene::~TestScene() {

}

#pragma endregion

#pragma region パブリック関数

#pragma endregion

#pragma region プライベート関数

void TestScene::LoadData() {
	GetManager()->GetGame()->GetTextureManager()->Load("Player", "Assets/AKAGE.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Sky", "Assets/Sky.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Plain", "Assets/Plain.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Mountain", "Assets/Mountain.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Tile", "Assets/Tile.png");
	GetManager()->GetGame()->GetTextureManager()->Load("RedBox", "Assets/RedBox.png");
	GetManager()->GetGame()->GetTileMapManager()->Load("TileMap", "Assets/test.csv");
	
	Player* _player = new Player(this);
	TileMapObject* _tileMapObject = new TileMapObject(this);
	_tileMapObject->SetOnTile(_player, -2);

	GetCamera()->Target(_player);
	new Background(this);
	
}

#pragma endregion