#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "Utilities/Math.h"
#include "GameObjects/GameObject.h"

class Input;
class Frame;
class SpriteComponent;
class BoxComponent2D;
class GameObject;
class VertexArray;
class Shader;
class TileMap;
class Texture;
class Camera;
class Player;
class PhysWorld2D;
class Renderer;
template <typename T>
class AssetManager;
class TileMapObject;

struct Ball {
    Vector2 pos;
    Vector2 vel;
};

class Game {
public:
    Game();
    ~Game();

    void StartThreads();

    bool Initialize(); // 初期化
    void RunLoop(); // メインループ
    void Shutdown(); // シャットダウン

    void AddObject(GameObject* object); // GameObject配列に追加
    void RemoveObject(GameObject* gameObject); // GameObject配列から削除

	Frame* GetFrame() const { return _frame; }
    Camera* GetCamera() const { return _camera; }
    Player* GetPlayer() const { return _player; }
    PhysWorld2D* GetPhysWorld() const { return _physWorld; }
    Renderer* GetRenderer() const { return _renderer; }
	AssetManager<Texture>* GetTextureManager() { return _textureManager; }
	AssetManager<TileMap>* GetTileMapManager()  { return _tileMapManager; }
    
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
    void GameLogicLoop();
    
private:
    void ProcessInput(); // 入力処理
    void Update(); // 更新処理
    void ProcessOutput(); // 出力処理
    
    void LoadData(); // データの読み込み
    void UnloadData(); // データの解放

    Camera* _camera;
    Player* _player;
	TileMapObject* _tileMapObject;
    Frame* _frame;
    Input* _input;

    PhysWorld2D* _physWorld;
    Renderer* _renderer;
    bool _isRunning;
    bool _isUpdating;

    
	AssetManager<Texture>* _textureManager;
	AssetManager<TileMap>* _tileMapManager;

    float _brightness;

    std::vector<GameObject*> _pendingObjects;
    std::vector<GameObject*> _objects;
};