#pragma once
#include <vector>
#include "GameObjects/GameObject.h"
#include "Scene/SubScene.h"

class SceneManager;
class Input;
class Camera;
class UIScreen;
class Shader;
class TextureComponent;
class UIScene;

class Scene {
public:

    enum class STATE {
        ACTIVE,
        INACTIVE,
        CLOSE
    };

    Scene(SceneManager* manager);

    virtual ~Scene();

    virtual void Initialize() {};
    void ProcessInput(Input* input);
	virtual void InputScene(Input* input) {};
    void Update(float deltaTime);
	virtual void UpdateScene(float deltaTime) {};
    void PhysUpdate(float deltaTime);
    void Output();
    virtual void Shutdown();

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

    void AddObject(GameObject* object); // GameObject配列に追加
    void RemoveObject(GameObject* gameObject);  // GameObject配列から削除

    void DestroyObject(GameObject* object);

    SceneManager* GetManager() const { return _manager; }

	STATE GetState() const { return _state; }
	void SetState(STATE state) { _state = state; }

    virtual void LoadData() {}; // データの読み込み
    virtual void UnloadData(); // データの解放

    Camera* GetCamera() const { return _camera; }

protected:

    std::vector<GameObject*> _pendingObjects;
    std::vector<GameObject*> _objects;

    Camera* _camera;
    STATE _state;
    SceneManager* _manager;

    bool _isUpdating;

};