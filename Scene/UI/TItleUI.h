#pragma once
#include "Scene/UI/UIScene.h"

class Button;

class TitleUI : public UIScene {
public:
	TitleUI(MainScene* scene);
	~TitleUI() override;

	//void Initialize() override;

private:
	
	void LoadData() override;

	Button* _button;

};