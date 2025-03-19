#include "LarvaEngine/Examples/Game/Test/TestScene.h"
#include "LarvaEngine/GameObjects/Player.h"
#include "LarvaEngine/GameObjects/TileMapObject.h"
#include "LarvaEngine/GameObjects/Background.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Core/Resources/TileMap.h"
#include "LarvaEngine/Examples/Game/Test/PauseMenu.h"
#include "LarvaEngine/Core/Events/Input.h"
#include "LarvaEngine/Examples/Game/Test/PlaySubScene.h"
#include "LarvaEngine/GameObjects/UI/Text.h"
#include "LarvaEngine/Examples/Game/Test/HUD.h"
#include "LarvaEngine/GameObjects/Particle.h"
#include "LarvaEngine/Renderer/Renderer.h"

#pragma region コンストラクタ:デストラクタ

Example::TestScene::TestScene(SceneManager* manager)
	: MainScene(manager) {

}

Example::TestScene::~TestScene() {

}

#pragma endregion

#pragma region パブリック関数

#pragma endregion

#pragma region プライベート関数

void Example::TestScene::Initialize() {
	LoadData();
}

void Example::TestScene::LoadData() {
	GetManager()->GetGame()->GetTextureManager()->Load("Player", "Assets/Textures/16Player.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Sky", "Assets/Textures/Sky.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Plain", "Assets/Textures/Plain.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Mountain", "Assets/Textures/Mountain.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg0", "Assets/Textures/bg_1.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg1", "Assets/Textures/bg_2.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg2", "Assets/Textures/bg_3.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg3", "Assets/Textures/bg_4.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg4", "Assets/Textures/bg_5.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Bg5", "Assets/Textures/bg_6.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Tile", "Assets/Textures/16Tile.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Fire", "Assets/Textures/fire.png");
	GetManager()->GetGame()->GetTextureManager()->Load("Sun", "Assets/Textures/Sun.png");
	GetManager()->GetGame()->GetTextureManager()->Load("RedBox", "Assets/Textures/RedBox.png");
	GetManager()->GetGame()->GetTileMapManager()->Load("TileMap", "Assets/TileMaps/TileMap.csv");

	_player = new Player(this);
	TileMapObject* _tileMapObject = new TileMapObject(this);
	
	Particle* particle = new Particle(this);
	particle->Position(Vector2(100, 50));
	particle->SetTexture("Fire");
	particle->SetSpriteSheet(32,32,0,0,16, 0.08f,true);
	particle->SetLight(Vector3(1,0.2,0.2), 0.5f, 20.f, Vector2(0, -5));

	_tileMapObject->SetOnTile(_player, -2);
	_camera = new Camera(this);
	GetCamera()->Position(_player->Position());
	GetCamera()->Target(_player);
	//

	Particle* sun = new Particle(this, 0);
	sun->Position(Vector2(-120,70));
	sun->SetTexture("Sun");
	sun->SetSpriteSheet(64, 64, 0, 0, 15, 0.2f, true);
	sun->SetLight(Vector3(1, 0.2, 0.2), 0.5f, 30.f, Vector2(0, 0));

	new Background(this);

	_currentSubScene = new Example::PlaySubScene(this, _player);
	_currentSubScene->Initialize();
	CreateUIScene<HUD>();

	GetManager()->GetGame()->GetRenderer()->SetAllAmbientLightFactor(AmbientLight(Vector3(1.0,1.0,1.0), 0.9));


}

#pragma endregion