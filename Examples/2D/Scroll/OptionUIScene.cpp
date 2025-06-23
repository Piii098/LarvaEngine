#include "Examples/2D/Scroll/OptionUIScene.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"

Example2DScroll::OptionUIScene::OptionUIScene(MainScene& parent)
	: UIScene(parent)
{
}

Example2DScroll::OptionUIScene::~OptionUIScene()
{
}

void Example2DScroll::OptionUIScene::InputUI(const InputAction& input)
{
	_buttonGroup.InputProcess(input);
}

void Example2DScroll::OptionUIScene::UpdateUI(float deltaTime)
{
}

void Example2DScroll::OptionUIScene::LoadObjects()
{
	GameObject& background = CreateGameObject<GameObject>();
	SpriteComponent& spri = background.CreateComponent<SpriteComponent>(10);
	spri.SetTexture("Rectangle");
	background.Scale(3.f);
	spri.TexScale(Vector2(100, 100));
	//spri.TexScale(Vector2(1.8, 1));

	Button& back = CreateGameObject<Button>("Button");
	SpriteComponent* spri2 = back.GetComponent<SpriteComponent>();
	spri2->TexScale(Vector2(0.8, 1));
	back.SetText("Back", Vector3(0, 0, 0), 20);
	back.Position2D(Vector2(0, -200));
	back.SetOnClick([this]() {
		_state = STATE::CLOSE;
		});
	back.AddToGroup(_buttonGroup);
}
