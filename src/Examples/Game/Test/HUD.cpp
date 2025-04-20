#include "LarvaEngine/Examples/Game/Test/HUD.h"
#include "LarvaEngine/GameObjects/UI/DataText.h"

Example::HUD::HUD(MainScene& parent)
	: UIScene(parent) {
}

Example::HUD::~HUD() {
}

void Example::HUD::LoadData() {
	
	DataText& dataTex = CreateGameObject<DataText>("DelaSuko", Vector3::fromIntRGB(73, 69, 68), 50);
	dataTex.SetData("Player.Position.X");
	dataTex.SetLabel("プレイヤー X");
	dataTex.Position(Vector2Int(-450, 170));
	dataTex.SetAlignLeft();

	dataTex.SetData("mouse.pos.X");
	dataTex.SetLabel("mouse.pos.X");
	dataTex.Position(Vector2Int(-450, 200));
	dataTex.SetAlignLeft();

	Text text = CreateGameObject<Text>("DelaSuko", Color::White, 50, "HUD");
	text.Position(Vector2Int(-450, -170));
	text.SetAlignLeft();
}
