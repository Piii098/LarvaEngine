#pragma once
#include "Scene/Scene.h"


class TestScene : public Scene {
public:
	TestScene(SceneManager* manager);
	~TestScene() override;
	
	void InputScene(Input* input) override;
	void LoadData() override;

private:
	
};