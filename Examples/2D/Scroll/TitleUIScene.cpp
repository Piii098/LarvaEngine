#include "Examples/2D/Scroll/TitleUIScene.h"
#include "Examples/2D/Scroll/AnimatedButton.h"
#include "LarvaEngine/GameObjects/UI/Button.h"
#include "LarvaEngine/GameObjects/UI/Text.h"
#include "Examples/2D/Scroll/GameMainScene.h"
#include "Examples/2D/Scroll/TitleMainScene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "Examples/2D/Scroll/OptionUIScene.h"

Example2DScroll::TitleUIScene::TitleUIScene(MainScene& parent)
	: UIScene(parent) {
}

Example2DScroll::TitleUIScene::~TitleUIScene() {

}

void Example2DScroll::TitleUIScene::InputUI(const InputAction& input) {
	_buttonGroup.InputProcess(input);
}

void Example2DScroll::TitleUIScene::UpdateUI(float deltaTime) {
	
}

void Example2DScroll::TitleUIScene::LoadObjects() {
	Text& Logo = CreateGameObject<Text>("MOBO", Vector3(1, 1, 1), 100, "2D Scroll Sample");
	Logo.Position(Vector3(0, 200, 0));

	AnimatedButton& start = CreateGameObject<AnimatedButton>("Button");
	AnimatedButton& option = CreateGameObject<AnimatedButton>("Button");

	start.AddToGroup(_buttonGroup);
	option.AddToGroup(_buttonGroup);

	start.SetText("Start", Vector3(0, 0, 0), 20);
	
	start.StartAnimation(Vector2Int(-1300, 0), Vector2Int(0, 0), 1.f);
	option.SetText("Option", Vector3(0, 0, 0), 20);
	option.StartAnimation(Vector2Int(-1300, -100), Vector2Int(0, -100), 1.f, 0.2);

	start.SetOnClick([this]() {
			_manager.GetGame().GetSceneManager().RequestSceneChange<Example2DScroll::GameMainScene>();
		});

	option.SetOnClick([this]() {
			_parent.CreateUIScene<Example2DScroll::OptionUIScene>();
		});
}
