#pragma once
#include "Scene/UI/UIScene.h"

class TitleUI : public UIScene {
public:
	TitleUI(SceneManager* manager);
	~TitleUI() override;

	void Initialize() override;

	void LoadData() override;
private:

};