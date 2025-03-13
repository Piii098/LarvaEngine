#pragma once
#include "Scene/MainScene.h"	

class TitleScene : public MainScene {
public:
	TitleScene(SceneManager* manager);
	~TitleScene() override;


private:

	void LoadData() override;

};