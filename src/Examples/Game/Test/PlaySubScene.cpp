#include "LarvaEngine/Examples/Game/Test/PlaySubScene.h"
#include "LarvaEngine/GameObjects/Player.h"
#include "LarvaEngine/Components/Physics/MoveInputComponent.h"
#include "LarvaEngine/Core/Events/Input.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Examples/Game/Test/PauseMenu.h"
#include "LarvaEngine/Core/MainScene.h"

#pragma region コンストラクタデストラクタ

Example:: PlaySubScene::PlaySubScene(MainScene* parent, Player* player)
	: SubScene(parent) 
	, _player(player){
}

Example::PlaySubScene::~PlaySubScene() {
}

#pragma endregion

#pragma region パブリック関数

void Example::PlaySubScene::Initialize() {
	_player->GetComponent<MoveInputComponent>()->SetState(Component::STATE::ACTIVE);
	
}

void Example::PlaySubScene::InputScene(Input* input) {
	if (input->IsInputDown(InputMap::INPUT_START)) {
		SDL_Log("uiOpen");
		_parent->CreateUIScene<PauseMenu>();
	}
}

void Example::PlaySubScene::UpdateScene(float deltaTime) {

}

#pragma endregion