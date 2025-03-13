#include "Scene/Game/PlaySubScene.h"
#include "GameObjects/Player.h"
#include "Components/MoveInputComponent.h"
#include "Utilities/Input.h"
#include "Scene/Scene.h"
#include "Scene/UI/PauseMenu.h"
#include "Scene/MainScene.h"

#pragma region コンストラクタデストラクタ

test:: PlaySubScene::PlaySubScene(MainScene* parent, Player* player)
	: SubScene(parent) 
	, _player(player){
}

test::PlaySubScene::~PlaySubScene() {
}

#pragma endregion

#pragma region パブリック関数

void test::PlaySubScene::Initialize() {
	_player->GetComponent<MoveInputComponent>()->SetState(Component::STATE::ACTIVE);
	
}

void test::PlaySubScene::InputScene(Input* input) {
	if (input->IsInputDown(InputMap::INPUT_START)) {
		SDL_Log("uiOpen");
		_parent->CreateUIScene<PauseMenu>();
	}
}

void test::PlaySubScene::UpdateScene(float deltaTime) {

}

#pragma endregion