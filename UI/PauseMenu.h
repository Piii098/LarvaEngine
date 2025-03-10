#pragma once
#include "UI/UIScreen.h"

class PauseMenu : public UIScreen {
public:
	PauseMenu(Scene* scene);
	~PauseMenu() override;

	void ProcessInput(Input* input) override;
	void Update(float deltaTime) override;
	void Render(Shader* shader) override;
private:
	Texture* _test;
};