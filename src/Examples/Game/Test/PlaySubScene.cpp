#include "LarvaEngine/Examples/Game/Test/PlaySubScene.h"
#include "LarvaEngine/GameObjects/Player.h"
#include "LarvaEngine/Components/Physics/MoveInputComponent.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Examples/Game/Test/PauseMenu.h"
#include "LarvaEngine/Core/MainScene.h"
#include "LarvaEngine/Input/InputAction.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"

#pragma region コンストラクタデストラクタ

Example:: PlaySubScene::PlaySubScene(MainScene& parent, Player* player)
	: SubScene(parent) 
	, _player(player){
}

Example::PlaySubScene::~PlaySubScene() {
}

#pragma endregion

#pragma region パブリック関数

void Example::PlaySubScene::Initialize() {
	_player->GetComponent<MoveInputComponent>()->State(Component::STATE::ACTIVE);
	_parent.SetData("mouse.pos.X", 1.0f);
	_parent.SetDataUpdate("mouse.pos.X", [this]() {
		return _parent.GetManager().GetGame().GetInputAction().GetMousePosition().x;
		});

}

void Example::PlaySubScene::InputScene(const InputAction& input) {
	if (input.IsKeyDown(SDL_SCANCODE_TAB)) {
		SDL_Log("uiOpen");
		_parent.CreateUIScene<PauseMenu>();
	}
}

void Example::PlaySubScene::UpdateScene(float deltaTime) {

}

#pragma endregion