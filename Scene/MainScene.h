#pragma once
#include "Scene/Scene.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include "GameObjects/GameObject.h"
#include "Scene/SubScene.h"
#include "Utilities/DataTypes.h"
#include <string>
#include <variant>

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

    void SetData(const std::string& key, GameTypes::DataValue value);
    GameTypes::DataValue GetData(const std::string& key);
    void SetGetter(const std::string& key, std::function<GameTypes::DataValue()> getter);
       
protected:
    Camera* _camera;

	SubScene* _currentSubScene;
	std::vector<UIScene*> _uiScenes;
	std::unordered_map<std::string, GameTypes::DataValue> _data;
    std::unordered_map<std::string, std::function<GameTypes::DataValue()>> _getter;
};