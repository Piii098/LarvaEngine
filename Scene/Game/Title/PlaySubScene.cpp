#include "Scene/Game/Title/PlaySubScene.h"
#include "GameObjects/Player.h"
#include "Utilities/Input.h"
#include "Scene/UI/PauseMenu.h"
#include "Scene/UI/TitleUI.h"
#include "Scene/MainScene.h"

Title::PlaySubScene::PlaySubScene(MainScene* parent)
	: SubScene(parent) {
}

Title::PlaySubScene::~PlaySubScene() {
}

void Title::PlaySubScene::Initialize() {
	_parent->CreateUIScene<TitleUI>();
}

void Title::PlaySubScene::InputScene(Input* input) {

}

void Title::PlaySubScene::UpdateScene(float deltaTime) {

}