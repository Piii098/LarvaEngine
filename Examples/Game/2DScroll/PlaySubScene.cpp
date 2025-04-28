#include "Examples/Game/2DScroll/PlaySubScene.h"
#include "Examples/Game/2DScroll/Player.h"
#include "LarvaEngine/Components/Physics/MoveInputComponent.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/MainScene.h"
#include "Examples/Game/2DScroll/GameMainScene.h"

Example2DScroll::PlaySubScene::PlaySubScene(MainScene& mainScene, Player& player)
	: SubScene(mainScene)
	, _player(player){

}

Example2DScroll::PlaySubScene::~PlaySubScene() {

}

void Example2DScroll::PlaySubScene::Initialize() {
	_player.GetComponent<MoveInputComponent>()->State(Component::STATE::ACTIVE);
	
}

void Example2DScroll::PlaySubScene::UpdateScene(float deltaTime) {
	if (_player.Position().y < -100.0f) {
		_parent.GetManager().RequestSceneChange<GameMainScene>();
	}
}