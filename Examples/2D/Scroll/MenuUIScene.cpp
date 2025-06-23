#include "Examples/2D/Scroll/MenuUIScene.h"
#include "LarvaEngine/GameObjects/UI/Text.h"
#include "Examples/2D/Scroll/AnimatedButton.h"
#include "Examples/2D/Scroll/TitleMainScene.h"
#include "LarvaEngine/GameObjects/UI/Button.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "Examples/2D/Scroll/GameMainScene.h"

Example2DScroll::MenuUIScene::MenuUIScene(MainScene& parent)
	: UIScene(parent) {
}

Example2DScroll::MenuUIScene::~MenuUIScene() {
}

void Example2DScroll::MenuUIScene::InputUI(const InputAction& input) {
	_buttonGroup.InputProcess(input);
}

void Example2DScroll::MenuUIScene::LoadData() {
	
}

void Example2DScroll::MenuUIScene::LoadObjects() {
	Text& header = CreateGameObject<Text>("MOBO", Vector3(1, 1, 1), 100, "MENU");
	header.Position(Vector3(-500, 200, 0));

	AnimatedButton& option = CreateGameObject<AnimatedButton>("Button");
	AnimatedButton& back = CreateGameObject<AnimatedButton>("Button");

	option.AddToGroup(_buttonGroup);
	back.AddToGroup(_buttonGroup);

	option.SetText("Option", Vector3(0, 0, 0), 20);
	back.SetText("Back", Vector3(0, 0, 0), 20);

	option.StartAnimation(Vector2(-1300, -100), Vector2(-500, -100), 1.f);
	back.StartAnimation(Vector2(-1300, -200), Vector2(-500, -200), 1.f, 0.2);

	option.SetOnClick([this]() {
		
		});

	back.SetOnClick([this]() {
		_manager.GetGame().GetSceneManager().RequestSceneChange<Example2DScroll::TitleMainScene>();
		});

	
}