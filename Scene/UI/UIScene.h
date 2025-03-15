#pragma once
#include "Utilities/Math.h"
#include "Scene/Scene.h"
#include <string>

class Shader;
class Input;
class Texture;
class Game;
class GameScene;
class Font;

class UIScene : public Scene {
public:

	UIScene(MainScene* scene);
	virtual ~UIScene();

	virtual void Initialize() override;
	void InputScene(Input* input) override;
	virtual void InputUI(Input* input) {};
	void UpdateScene(float deltaTime) override;
	virtual void UpdateUI(float deltaTime) {};
	void Render(Shader* shader);

protected:

	MainScene* _scene;
	virtual void LoadData() override {};

};