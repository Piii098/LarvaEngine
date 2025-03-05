#pragma once
#include <vector>
#include "GameObjects/GameObject.h"

class SceneManager;
class Input;
class Camera;

class Scene {
public:

	Scene(SceneManager* maneger);
	virtual ~Scene();

	void Initialize();
	void ProcessInput(Input* input);
	void Update(float deltaTime);
	void PhysUpdate(float deltaTime);
	void Output();
	void Shutdown();

	virtual void LoadData(); // データの読み込み
	void UnloadData(); // データの解放


	void AddObject(GameObject* object); // GameObject配列に追加
	void RemoveObject(GameObject* gameObject); // GameObject配列から削除

	template <typename T, typename... Args>
	T* CreateGameObject(Args&&... args) {
		T* gameObject = new T(this, std::forward<Args>(args)...);
		return gameObject;
	}

	template <typename T, typename... Args>
	T* CreateChildObject(GameObject* parent, Args&&... args) {
		T* childObject = new T(this, std::forward<Args>(args)...);
		parent->AddChildren(childObject);
		return childObject;
	}

	void DestroyObject(GameObject* object);

	SceneManager* GetManager() const { return _manager; }
	Camera* GetCamera() const { return _camera; }

private:

	Camera* _camera;
	SceneManager* _manager;
	bool _isUpdating;

	std::vector<GameObject*> _pendingObjects;
	std::vector<GameObject*> _objects;
};