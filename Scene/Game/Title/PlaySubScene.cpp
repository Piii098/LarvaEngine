#include "Scene/Game/Title/PlaySubScene.h"
#include "GameObjects/Player.h"
#include "Utilities/Input.h"
#include "Scene/UI/PauseMenu.h"
#include "Scene/MainScene.h"

Title::PlaySubScene::PlaySubScene(MainScene* parent)
	: SubScene(parent) {
}

Title::PlaySubScene::~PlaySubScene() {
}

void Title::PlaySubScene::Initialize() {

}

void Title::PlaySubScene::InputScene(Input* input) {
	if (input->IsInputDown(InputMap::INPUT_START)) {
		SDL_Log("uiOpen");
		_parent->CreateUIScene<PauseMenu>();
	}

}

void Title::PlaySubScene::UpdateScene(float deltaTime) {

}