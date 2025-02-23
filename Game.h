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

	bool Initialize(); // ��������
	void RunLoop(); // ���[�v�쓮
	void Shutdown(); // �V���b�h�_�E��

	void AddObject(GameObject* object); // GameObject�z��ɒǉ�
	void RemoveObject(GameObject* gameObject); // GameObject�z�񂩂�폜

	Camera* GetCamera() { return _camera; };
	Player* GetPlayer() { return _player; };
	PhysWorld2D* GetPhysWorld() { return _physWorld; };
	Renderer* GetRenderer() { return _renderer; };
	

private:
	/*���[�v�@�\*/
	void ProcessInput();
	void Update();
	void ProcessOutput();
	
	void LoadData();

	/*�V�X�e���ϐ�*/
	Camera* _camera;
	Player* _player;
	Frame* _frame;
	Input* _input;

	PhysWorld2D* _physWorld;
	Renderer* _renderer;
	bool _isRunning; //���[�v�p��
	bool _isUpdating;

	std::unordered_map<std::string, Texture*> _textures;
	std::vector<SpriteComponent*> _sprites;
	std::vector<GameObject*> _pendingObjects; //�ۗ����̃I�u�W�F�N�g�z��
	std::vector<GameObject*> _objects; //���s�Ώۂ̃I�u�W�F�N�g�z��


};