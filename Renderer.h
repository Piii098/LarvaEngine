#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <SDL3/SDL.h>

class Game;
class VertexArray;
class Shader;
class Texture;
class SpriteComponent;
class BGComponent;
class TextureComponent;

class Renderer {
public:

	struct Line {
		Line() = default;
		Vector2 start;
		Vector2 end;
		Vector3 color;
	};

	Renderer(Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight, float dotWidth, float dotHeight);
	void Shutdown();

	void UnloadData();

	void Draw();

	void AddSprite(SpriteComponent* sprite);
	void RemoveSprite(SpriteComponent* sprite);
	void AddBackground(BGComponent* background);
	void RemoveBackground(BGComponent* background);
	void AddTexture(TextureComponent* texture);
	void RemoveTexture(TextureComponent* texture);

	void SetLightPos(Vector2 lightPos) { _lightPos = lightPos; };
private:

	bool InitializeFrameBuffer();

	void DrawBackground();
	void DrawSprite();

	void UpScale();
	void ApplyBloom(bool& horizontal);
	bool LoadShaders();
	void CreateSpriteVerts();

	unsigned int _colorBuffer[2];
	unsigned int _hdrFBO;
	VertexArray* _screenVerts;
	bool _isBloom;

	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];

	Shader* _blurShader;               // �K�E�V�A���u���[�p�V�F�[�_�[
	Shader* _bloomFinalShader;         // �ŏI�����p�V�F�[�_�[

	// �u���[���ݒ�
	float _bloomThreshold = 1.0f;      // ���邢�����Ƃ݂Ȃ�臒l
	float _bloomIntensity = 0.8f;      // �u���[���̋��x

	std::vector<BGComponent*> _backgrounds;
	std::vector<SpriteComponent*> _sprites;

	Game* _game;

	SDL_Window* _window; //�E�B���h�E
	SDL_GLContext _context;
	VertexArray* _spriteVerts;
	Shader* _spriteShader;
	Shader* _backgroundShader;
	Shader* _frameBufferShader;

	unsigned int _lowResFrameBuffer;
	unsigned int _lowResFrameFBO;
	Shader* _upscaleShader;

	float _screenWidth;
	float _screenHeight;
	float _lowResWidth;
	float _lowResHeight;

	Vector2 _lightPos;

};