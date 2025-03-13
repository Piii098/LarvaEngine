#include "Scene/UI/PauseMenu.h"
#include "Scene/Scene.h"
#include "Utilities/Input.h"
#include "Scene/SceneManager.h"
#include "GameObjects/UI/Text.h"
#include "Core/Game.h"
#include "AssetManagers/AssetManager.h"
#include "GameObjects/Player.h"

#pragma region コンストラクタ:デストラクタ

PauseMenu::PauseMenu(MainScene* parent)
	: UIScene(parent) {
	_scene->SetState(Scene::STATE::INACTIVE);
	
}

PauseMenu::~PauseMenu() {
	_scene->SetState(Scene::STATE::ACTIVE);
}

#pragma endregion

#pragma region パブリック関数


#pragma endregion

#pragma region プライベート関数

void PauseMenu::Initialize() {

	LoadData();
}

void PauseMenu::InputUI(Input* input) {

	if (input->IsInputDown(InputMap::INPUT_BACK)) {
		SDL_Log("uiClose");
		_state = STATE::CLOSE;

	}

}

void PauseMenu::LoadData() {

	Text* shade = new Text(this, "DelaSuko", Color::Black, 90, "PAUSE");
	shade->Position(Vector2Int(5, -5));

	_text = new Text(this, "DelaSuko", Color::White, 90, "PAUSE");
	_text->CreateOutline(Color::Black);
	_text->Position(Vector2Int(-5, 5));
	//_player = new Player(this);
	//_player->Position(Vector2Int(10, 10));
}

#pragma endregion