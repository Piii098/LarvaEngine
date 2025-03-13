#pragma once
#include "Scene/Scene.h"
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

class MainScene : public Scene {
public:

    MainScene(SceneManager* manager);

    virtual ~MainScene() override;

    virtual void Initialize() override;
	virtual void InputScene(Input* input) override;
	virtual void UpdateScene(float deltaTime) override;
    void Render(Shader* shader, int bufferLayer);
	void RenderUIs(Shader* shader);
    virtual void Shutdown() override;

    template <typename T, typename... Args>
    T* CreateUIScene(Args&&... args) {
        T* uiScene = new T(this, std::forward<Args>(args)...);
		uiScene->Initialize();
        return uiScene;
    }

    template <typename T, typename... Args>
    T* ChangeSubScene(Args&& ...args) {
        if (_currentSubScene != nullptr) {
            _currentSubScene->Shutdown();
            delete _currentSubScene;
            _currentSubScene = nullptr;
        }

        T* tempScene = new T(this, std::forward<Args>(args)...);
        _currentSubScene = tempScene;
        _currentSubScene->Initialize();
        return tempScene;
    }

	void AddUIScene(UIScene* uiScene);
	void RemoveUIScene(UIScene* uiScene);

    std::vector<UIScene*> GetUIScenes() const { return _uiScenes; }

    virtual void LoadData() override {}; // データの読み込み
    virtual void UnloadData() override {}; // データの解放
    Camera* GetCamera() const { return _camera; }

protected:
    Camera* _camera;

	SubScene* _currentSubScene;
	std::vector<UIScene*> _uiScenes;

};