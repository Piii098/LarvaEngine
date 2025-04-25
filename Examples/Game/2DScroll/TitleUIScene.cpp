#include "Examples/Game/2DScroll/TitleUIScene.h"
#include "LarvaEngine/GameObjects/UI/Button.h"
#include "Examples/Game/2DScroll/GameMainScene.h"
#include "Examples/Game/2DScroll/TitleMainScene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"

Example2DScroll::TitleUIScene::TitleUIScene(MainScene& parent)
	: UIScene(parent) {
}

Example2DScroll::TitleUIScene::~TitleUIScene() {

}

void Example2DScroll::TitleUIScene::LoadData() {
	Button& button = CreateGameObject<Button>("Button");
	SDL_Log("Button Position: %f, %f", button.Position().x, button.Position().y);
	button.SetOnClick([this]() {

			_manager.GetGame().GetSceneManager().RequestSceneChange<Example2DScroll::GameMainScene>();
		});
}
