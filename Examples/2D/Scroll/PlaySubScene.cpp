#include "Examples/2D/Scroll/PlaySubScene.h"
#include "Examples/2D/Scroll/Player.h"
#include "LarvaEngine/Components/Physics/MoveInputComponent.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/MainScene.h"
#include "Examples/2D/Scroll/GameMainScene.h"
#include "Examples/2D/Scroll/MenuUIScene.h"
#include "LarvaEngine/Input/InputAction.h"

Example2DScroll::PlaySubScene::PlaySubScene(MainScene& mainScene, Player& player)
	: SubScene(mainScene)
	, _player(player){

}

Example2DScroll::PlaySubScene::~PlaySubScene() {

}

void Example2DScroll::PlaySubScene::InputScene(const InputAction& input) {

	if (input.IsActionDown("Menu") && !_isMenu) {
		_parent.CreateUIScene<MenuUIScene>();
		_player.GetComponent<MoveInputComponent>()->State(Component::STATE::INACTIVE);
		_isMenu = true;
	}
	else if (input.IsActionDown("Menu") && _isMenu) {
		_parent.GetUIScene<MenuUIScene>()->State(Scene::STATE::CLOSE);
		_player.GetComponent<MoveInputComponent>()->State(Component::STATE::ACTIVE);
		_isMenu = false;
	}
}

void Example2DScroll::PlaySubScene::Initialize() {
	_player.GetComponent<MoveInputComponent>()->State(Component::STATE::ACTIVE);
	
}

void Example2DScroll::PlaySubScene::UpdateScene(float deltaTime) {
	if (_player.Position().y < -100.0f) {
		_parent.GetManager().RequestSceneChange<GameMainScene>();
	}
}