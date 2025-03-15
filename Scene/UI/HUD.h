#pragma once
#include "Scene/UI/UIScene.h"

class DataText;
class Text;

class HUD : public UIScene {
public:
	HUD(MainScene* parent);
	~HUD() override;

	//void InputUI(Input* input) override;

	//void UpdateUI(float deltaTime) override;

private:
	void LoadData() override;
	DataText* _dataText;
	Text* _text;
};