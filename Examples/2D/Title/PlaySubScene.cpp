﻿#include "LarvaEngine/Examples/Game/Title/PlaySubScene.h"
#include "LarvaEngine/GameObjects/Player.h"
#include "LarvaEngine/Examples/Game/Test/PauseMenu.h"
#include "LarvaEngine/Examples/Game/Title/TitleUI.h"
#include "LarvaEngine/Core/MainScene.h"
#include "LarvaEngine/Input/InputAction.h"

ExampleTitle::PlaySubScene::PlaySubScene(MainScene& parent)
	: SubScene(parent) {
}

ExampleTitle::PlaySubScene::~PlaySubScene() {
}

void ExampleTitle::PlaySubScene::Initialize() {
	_parent.CreateUIScene<TitleUI>();
}

void ExampleTitle::PlaySubScene::InputScene(const InputAction& input) {

}

void ExampleTitle::PlaySubScene::UpdateScene(float deltaTime) {

}