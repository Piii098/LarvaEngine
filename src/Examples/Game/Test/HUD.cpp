#include "LarvaEngine/Examples/Game/Test/HUD.h"
#include "LarvaEngine/GameObjects/UI/DataText.h"

Example::HUD::HUD(MainScene* parent)
	: UIScene(parent) {
}

Example::HUD::~HUD() {
}

void Example::HUD::LoadData() {
	_dataText = new DataText(this, "DelaSuko", Vector3::fromIntRGB(73, 69, 68), 50);

	_dataText->SetData("Player.Position.X");
	_dataText->SetLabel("プレイヤー X");
	_dataText->Position(Vector2Int(-450, 170));
	_dataText->SetAlignLeft();

	_text = new Text(this, "DelaSuko", Color::White, 50, "HUD");
	_text->Position(Vector2Int(-450, -170));
	_text->SetAlignLeft();
}
