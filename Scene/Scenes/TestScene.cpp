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
#include "UI/PauseMenu.h"
#include "Utilities/Input.h"

#pragma region コンストラクタ:デストラクタ

TestScene::TestScene(SceneManager* manager) 
	: Scene(manager) {

}

TestScene::~TestScene() {

}

#pragma endregion

#pragma region パブリック関数

void TestScene::InputScene(Input* input) {
	if(input->IsInputDown(InputMap::INPUT_START) && _state != STATE::PAUSE) {
		PauseMenu* pauseMenu = new PauseMenu(this);
	}
}

#pragma endregion

#pragma region プライベート関数

void TestScene::LoadData() {
	GetManager()->GetGame()->GetTextureManager()->Load("Player", "Assets/16Player.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Sky", "Assets/Sky.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Plain", "Assets/Plain.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Mountain", "Assets/Mountain.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg0", "Assets/bg_0.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg1", "Assets/bg_1.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg2", "Assets/bg_2.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg3", "Assets/bg_3.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg4", "Assets/bg_4.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg5", "Assets/bg_5.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg6", "Assets/bg_6.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Tile", "Assets/16Tile.png");
	GetManager()->GetGame()->GetTextureManager()->Load("RedBox", "Assets/RedBox.png");
	GetManager()->GetGame()->GetTileMapManager()->Load("TileMap", "Assets/TileMap.csv");
	GetManager()->GetGame()->GetFontManager()->Load("DelaSuko", "Assets/DelaSukoGothicOne-R.ttf");

	Player* _player = new Player(this);
	TileMapObject* _tileMapObject = new TileMapObject(this);
	_tileMapObject->SetOnTile(_player, -2);

	GetCamera()->Target(_player);
	new Background(this);
	
}

#pragma endregion