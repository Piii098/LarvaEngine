#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "Utilities/Math.h"
#include "GameObjects/GameObject.h"
#include "Utilities/Font.h"

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

struct Ball {
    Vector2 pos;
    Vector2 vel;
};

class Game {
public:
    Game();
    ~Game();

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

    Camera* _camera;
    Player* _player;
	TileMapObject* _tileMapObject;
    Frame* _frame;
    Input* _input;
	//Font* _font;

    SceneManager* _sceneManager;
    PhysWorld2D* _physWorld;
	AudioSystem* _audioSystem;
    Renderer* _renderer;
    bool _isRunning;

	AssetManager<Texture>* _textureManager;
	AssetManager<TileMap>* _tileMapManager;
	AssetManager<Font>* _fontManager;

};