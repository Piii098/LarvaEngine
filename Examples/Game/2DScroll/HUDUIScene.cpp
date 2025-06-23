#include "Examples/Game/2DScroll/HUDUIScene.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"

Example2DScroll::HUDUIScene::HUDUIScene(MainScene& parent)
	: UIScene(parent) {
}

Example2DScroll::HUDUIScene::~HUDUIScene() {
}

void Example2DScroll::HUDUIScene::LoadData() {

}

void Example2DScroll::HUDUIScene::UpdateUI(float deltaTime) {
	_playerBarVec -= Vector2(0, 1) * deltaTime * 100;
	if (_playerBarVec.y < 0) {
		_playerBarVec.y = 0;
	}
	_playerBar->TexScale(_playerBarVec);
}

void Example2DScroll::HUDUIScene::LoadObjects() {
	auto& playerBar = CreateGameObject<GameObject>();
	auto& spriteComp = playerBar.CreateComponent<SpriteComponent>(10);
	_playerBarVec = Vector2(16, 200);
	playerBar.Position(Vector2Int(-600, 200));
	spriteComp.SetTexture("Rectangle");
	spriteComp.SetVerticalAlign(SpriteComponent::VerticalAlign::Bottom);
	spriteComp.TexScale(_playerBarVec);
	spriteComp.Color(Color::Red);
	_playerBar = &spriteComp;
}