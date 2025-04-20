#include <SDL3/SDL.h>
#include "LarvaEngine/Examples/Game/Title/TitleUI.h"
#include "LarvaEngine/GameObjects/UI/Button.h"
#include "LarvaEngine/Examples/Game/Test/TestScene.h"
#include "LarvaEngine/GameObjects/UI/Text.h"
#include "LarvaEngine/Core/SceneManager.h"

#pragma region コンストラクタデストラクタ

ExampleTitle::TitleUI::TitleUI(MainScene& scene)
	: UIScene(scene) {
}

ExampleTitle::TitleUI::~TitleUI() {
}

#pragma endregion

#pragma region パブリック関数



#pragma endregion

#pragma region プライベート関数

void ExampleTitle::TitleUI::UpdateScene(float deltaTime) {

	SDL_Log("SelectButton : %d", _button->GetSelectedButton());

}

void ExampleTitle::TitleUI::LoadData() {
	
}

#pragma endregion
