#pragma once
#include "UI/UIScreen.h"

class HUD : public UIScreen {
public:
	HUD(Scene* scene);
	~HUD() override;

	void Update(float deltaTime) override;
	void Render(Shader* shader) override;

};