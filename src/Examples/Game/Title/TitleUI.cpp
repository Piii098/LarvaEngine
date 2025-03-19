#include <SDL3/SDL.h>
#include "LarvaEngine/Examples/Game/Title/TitleUI.h"
#include "LarvaEngine/GameObjects/UI/Button.h"
#include "LarvaEngine/Examples/Game/Test/TestScene.h"
#include "LarvaEngine/GameObjects/UI/Text.h"
#include "LarvaEngine/Core/SceneManager.h"

#pragma region コンストラクタデストラクタ

ExampleTitle::TitleUI::TitleUI(MainScene* scene)
	: UIScene(scene) {
}

ExampleTitle::TitleUI::~TitleUI() {
}

#pragma endregion

#pragma region パブリック関数



#pragma endregion

#pragma region プライベート関数

void ExampleTitle::TitleUI::LoadData() {
	_button = new Button(this);
	_button->CreateButtonData();
	_button->SetText(0, "Start", "DelaSuko", 50, Color::White);
	_button->SetOnClick(0, [this]() {
		_scene->GetManager()->ChangeScene<Example::TestScene>();
	});
	_button->CreateButtonData();
	_button->SetText(1, "Quite", "DelaSuko", 50, Color::White);
	_button->SetOnClick(1, [this]() {
		_scene->GetManager()->ChangeScene<Example::TestScene>();
		});

	_button->SetOffset(Vector2(50,-50));

	_button->Position(Vector2Int(-300, -100));
	
}

#pragma endregion
