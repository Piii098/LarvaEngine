#include "LarvaEngine/Examples/Game/Test/PauseMenu.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/Events/Input.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/GameObjects/UI/Text.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/GameObjects/Player.h"

#pragma region コンストラクタ:デストラクタ

Example::PauseMenu::PauseMenu(MainScene* parent)
	: UIScene(parent) {
	_scene->SetState(Scene::STATE::INACTIVE);
	
}

Example::PauseMenu::~PauseMenu() {
	_scene->SetState(Scene::STATE::ACTIVE);
}

#pragma endregion

#pragma region パブリック関数


#pragma endregion

#pragma region プライベート関数

void Example::PauseMenu::Initialize() {

	LoadData();
}

void Example::PauseMenu::InputUI(Input* input) {

	if (input->IsInputDown(InputMap::INPUT_BACK)) {
		SDL_Log("uiClose");
		_state = STATE::CLOSE;

	}

}

void Example::PauseMenu::LoadData() {

	Text* shade = new Text(this, "DelaSuko", Color::Black, 90, "PAUSE");
	shade->Position(Vector2Int(5, -5));

	_text = new Text(this, "DelaSuko", Color::White, 90, "PAUSE");
	_text->CreateOutline(Color::Black);
	_text->Position(Vector2Int(-5, 5));
	//_player = new Player(this);
	//_player->Position(Vector2Int(10, 10));
}

#pragma endregion