#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Scene/MainScene.h"

class GameObject;
class UIScene;
class Game;

class SceneManager {
public:

	SceneManager(Game* game);
	~SceneManager();

	void ProcessInput(Input* input);
	void Update(float deltaTime);
	void PhysUpdate(float deltaTime);
	void Output();
	void Shutdown();

	template <typename T, typename... Args>
	T* ChangeScene(Args&& ...args) {
		if (_currentMainScene != nullptr) {
			_currentMainScene->Shutdown();
			delete _currentMainScene;
			_currentMainScene = nullptr;
		}

		T* tempScene = new T(this, std::forward<Args>(args)...);
		_currentMainScene = tempScene;
		_currentMainScene->Initialize();
		return tempScene;
	}

	void DestroyScene();

	void Initialize();
	
	Game* GetGame() const { return _game; }
	std::vector<UIScene*> GetCurrentUIScenes() const { return _currentMainScene->GetUIScenes(); }
	MainScene* GetCurrentMainScene() const { return _currentMainScene; }

private:

	void LoadData();

	Game* _game;
	MainScene* _currentMainScene;
	//std::vector<UIScene*> _currentUIScenes;

	std::unordered_map<std::string, GameScene*> _scenes;


	bool _isUpdating;
};