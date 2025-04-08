#include <SDL3/SDL.h>
#include "LarvaEngine/Examples/Game/Test/PauseMenu.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/GameObjects/UI/Text.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/GameObjects/Player.h"
#include "LarvaEngine/Input/InputAction.h"

#pragma region コンストラクタ:デストラクタ

Example::PauseMenu::PauseMenu(MainScene& parent)
	: UIScene(parent) {
	_parent.State(Scene::STATE::PAUSE);
	
}

Example::PauseMenu::~PauseMenu() {
	_parent.State(Scene::STATE::PAUSE);
}

#pragma endregion

#pragma region パブリック関数


#pragma endregion

#pragma region プライベート関数

void Example::PauseMenu::Initialize() {

	LoadData();
}

void Example::PauseMenu::InputUI(const InputAction& input) {

	if (input.IsKeyDown(SDL_SCANCODE_LSHIFT)) {
		SDL_Log("uiClose");
		_state = STATE::CLOSE;

	}

}

void Example::PauseMenu::LoadData() {

	Text& pause = CreateGameObject<Text>("DelaSuko", Color::White, 90, "PAUSE");
	pause.Position(Vector2Int(5, -5));

	Text& text = CreateGameObject<Text>("DelaSuko", Color::White, 90, "PAUSE");
	text.CreateOutline(Color::Black);
	text.Position(Vector2Int(-5, 5));
	//_player = new Player(this);
	//_player->Position(Vector2Int(10, 10));
}

#pragma endregion