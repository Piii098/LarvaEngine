#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Scene/Scene.h"

class GameObject;
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
		if (_currentScene != nullptr) {
			_currentScene->Shutdown();
			delete _currentScene;
			_currentScene = nullptr;
		}

		T* tempScene = new T(this, std::forward<Args>(args)...);
		_currentScene = tempScene;
		_currentScene->Initialize();
		return tempScene;
	}

	void DestroyScene();

	void Initialize();
	
	Game* GetGame() const { return _game; }

	Scene* GetCurrentScene() const { return _currentScene; }

private:

	Game* _game;
	Scene* _currentScene;

	std::unordered_map<std::string, Scene*> _scenes;

	bool _isUpdating;
};