#include "Scene/UI/HUD.h"
#include "GameObjects/UI/DataText.h"

HUD::HUD(MainScene* parent)
	: UIScene(parent) {
}

HUD::~HUD() {
}

void HUD::LoadData() {
	_dataText = new DataText(this, "DelaSuko", Color::White, 50);

	_dataText->SetData("Player.Position.X");
	_dataText->SetLabel("Player X");
	_dataText->Position(Vector2Int(-450, 200));
	_dataText->SetAlignLeft();

	_text = new Text(this, "DelaSuko", Color::White, 50, "HUD");
	_text->Position(Vector2Int(-450, -200));
	_text->SetAlignLeft();
}
