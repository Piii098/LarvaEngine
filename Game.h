#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "Math.h"

class Input;
class Frame;
class SpriteComponent;
class BoxComponent2D;
class GameObject;
class VertexArray;
class Shader;
class Texture;
class Camera;
class Player;
class PhysWorld2D;
class Renderer;

struct Ball {
	Vector2 pos;
	Vector2 vel;
};

class Game {
public:
	Game();
	~Game();

	bool Initialize(); // 初期処理
	void RunLoop(); // ループ駆動
	void Shutdown(); // シャッドダウン

	void AddObject(GameObject* object); // GameObject配列に追加
	void RemoveObject(GameObject* gameObject); // GameObject配列から削除

	Camera* GetCamera() { return _camera; };
	Player* GetPlayer() { return _player; };
	PhysWorld2D* GetPhysWorld() { return _physWorld; };
	Renderer* GetRenderer() { return _renderer; };
	

private:
	/*ループ機能*/
	void ProcessInput();
	void Update();
	void ProcessOutput();
	
	void LoadData();

	/*システム変数*/
	Camera* _camera;
	Player* _player;
	Frame* _frame;
	Input* _input;

	PhysWorld2D* _physWorld;
	Renderer* _renderer;
	bool _isRunning; //ループ継続
	bool _isUpdating;

	std::unordered_map<std::string, Texture*> _textures;
	std::vector<SpriteComponent*> _sprites;
	std::vector<GameObject*> _pendingObjects; //保留中のオブジェクト配列
	std::vector<GameObject*> _objects; //実行対象のオブジェクト配列


};