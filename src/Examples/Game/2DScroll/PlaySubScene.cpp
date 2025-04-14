#include "LarvaEngine/Examples/Game/2DScroll/PlaySubScene.h"
#include "LarvaEngine/Examples/Game/2DScroll/Player.h"
#include "LarvaEngine/Components/Physics/MoveInputComponent.h"

Example2DScroll::PlaySubScene::PlaySubScene(MainScene& mainScene, Player& player)
	: SubScene(mainScene)
	, _player(player){

}

Example2DScroll::PlaySubScene::~PlaySubScene() {

}

void Example2DScroll::PlaySubScene::Initialize() {
	_player.GetComponent<MoveInputComponent>()->State(Component::STATE::ACTIVE);
	
}