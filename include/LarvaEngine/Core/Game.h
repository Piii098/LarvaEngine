#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Resources/Font.h"
#include "LarvaEngine/Core/SceneManager.h"

class Input;
class Frame;
class SpriteComponent;
class BoxComponent2D;
class GameObject;
class SceneManager;
class TileMap;
class Texture;
class Camera;
class Player;
class PhysWorld2D;
class Renderer;
template <typename T>
class AssetManager;
class TileMapObject;
class Camera;
class AudioSystem;
class Audio;

class Game {
public:
    Game();
    ~Game();

    template <typename T>
    void SetInitialScene() {
        _initialSceneSetup = [](SceneManager* manager) {
            manager->InitializeWithScene<T>();
            };
    }

    void SetWindowsSize(int windowWidth, int windowHeight, int lowResWidth, int lowResHeight);
	void SetWindowsName(const std::string& name);

    bool Initialize(); // 初期化

    void RunLoop(); // メインループ
    void Shutdown(); // シャットダウン

	Frame* GetFrame() const { return _frame; }
    Player* GetPlayer() const { return _player; }
    PhysWorld2D* GetPhysWorld() const { return _physWorld; }
    Renderer* GetRenderer() const { return _renderer; }
	AudioSystem* GetAudioSystem() const { return _audioSystem; }
	AssetManager<Texture>* GetTextureManager() { return _textureManager; }
	AssetManager<TileMap>* GetTileMapManager()  { return _tileMapManager; }
    AssetManager<Font>* GetFontManager() { return _fontManager; };
	AssetManager<Audio>* GetAudioManager() { return _audioManager; }

	Frame* GetFrame() { return _frame; }
	SceneManager* GetSceneManager() { return _sceneManager; }


private:
    void ProcessInput(); // 入力処理
    void Update(); // 更新処理
	void PhysUpdate(); // 物理更新処理
	void UpdateScene(); // 更新処理
    void ProcessOutput(); // 出力処理
	void Render(); // 描画処理
    
    void LoadScene(); // データの読み込み
    void UnloadData(); // データの解放

	int _windowWidth;
	int _windowHeight;
	int _lowResWidth;
	int _lowResHeight;
	std::string _windowName;

    Camera* _camera;
    Player* _player;
	TileMapObject* _tileMapObject;
    Frame* _frame;
    Input* _input;
	//Font* _font;

    SceneManager* _sceneManager;
    std::function<void(SceneManager*)> _initialSceneSetup;
    PhysWorld2D* _physWorld;
	AudioSystem* _audioSystem;
    Renderer* _renderer;
    bool _isRunning;

	AssetManager<Texture>* _textureManager;
	AssetManager<TileMap>* _tileMapManager;
	AssetManager<Font>* _fontManager;
	AssetManager<Audio>* _audioManager;

};