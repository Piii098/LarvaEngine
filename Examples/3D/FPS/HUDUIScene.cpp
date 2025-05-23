#include "Examples/3D/FPS/HUDUIScene.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"
#include "LarvaEngine/GameObjects/UI/Text.h"

Example3DFPS::HUDUIScene::HUDUIScene(MainScene& parent)
	: UIScene(parent)
{
}

Example3DFPS::HUDUIScene::~HUDUIScene()
{
}

void Example3DFPS::HUDUIScene::LoadData()
{

}

void Example3DFPS::HUDUIScene::LoadObjects()
{
	Text& text = CreateGameObject<Text>("MOBO", Vector3(1, 1, 1), 100, "FPS");
	text.Position(Vector3(0, 200, 0));
}