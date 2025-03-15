#include <SDL3/SDL.h>
#include "Scene/UI/TitleUI.h"
#include "GameObjects/UI/Button.h"
#include "Scene/Game/TestScene.h"
#include "Scene/SceneManager.h"

#pragma region コンストラクタデストラクタ

TitleUI::TitleUI(MainScene* scene)
	: UIScene(scene) {
}

TitleUI::~TitleUI() {
}

#pragma endregion

#pragma region パブリック関数



#pragma endregion

#pragma region プライベート関数

void TitleUI::LoadData() {
	_button = new Button(this);
	_button->CreateButtonData();
	_button->SetText(0, "Start", "DelaSuko", 50, Color::White);
	_button->SetOnClick(0, [this]() {
		_scene->GetManager()->ChangeScene<TestScene>();
	});
	_button->CreateButtonData();
	_button->SetText(1, "Quite", "DelaSuko", 50, Color::White);
	_button->SetOnClick(1, [this]() {
		_scene->GetManager()->ChangeScene<TestScene>();
		});

	_button->SetOffset(Vector2(50,-50));

	_button->Position(Vector2Int(-300, -100));

}

#pragma endregion
