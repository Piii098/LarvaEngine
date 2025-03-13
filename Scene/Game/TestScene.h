#pragma once
#include "Scene/MainScene.h"

class Player;

class TestScene : public MainScene {
public:
	TestScene(SceneManager* manager);
	~TestScene() override;

	void Initialize() override;

	void LoadData() override;

	Player* GetPlayer() const { return _player; }

private:
	Player* _player;
};