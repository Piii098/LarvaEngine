#pragma once
#include "Scene/Scene.h"


class TestScene : public Scene {
public:
	TestScene(SceneManager* manager);
	~TestScene() override;
	
	void LoadData() override;

private:
	
};