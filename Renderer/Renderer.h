#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include "Utilities/Math.h"

class Game;
class VertexArray;
class Shader;
class Texture;
class SpriteComponent;
class BGComponent;
class TextureComponent;

class Renderer {
public:

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

	void SetLightPos(Vector2Int lightPos) { _lightPos = lightPos; };
private:

	bool InitializeFrameBuffer();

	void DrawBackground();
	void DrawSprite();

	void ClearScreen();
	void SetupShaders();
	void DrawScene();
	void ApplyBloomEffect();
	void FinalizeFrame();
	void SwapWindow();


	void UpScale();
	void ApplyBloom(bool& horizontal);
	bool LoadShaders();
	void CreateSpriteVerts();

	unsigned int _colorBuffer[2];
	unsigned int _hdrFBO;
	VertexArray* _screenVerts;
	bool _isBloom;

	unsigned int _pingpongFBO[2];
	unsigned int _pingpongBuffer[2];

	Shader* _blurShader;               // ガウシアンブラー用シェーダー
	Shader* _bloomFinalShader;         // 最終合成用シェーダー

	// ブルーム設定
	float _bloomThreshold = 1.0f;      // 明るい部分とみなす閾値
	float _bloomIntensity = 0.8f;      // ブルームの強度

	std::vector<BGComponent*> _backgrounds;
	std::vector<SpriteComponent*> _sprites;

	Game* _game;

	SDL_Window* _window; //ウィンドウ
	SDL_GLContext _context;
	VertexArray* _spriteVerts;
	Shader* _spriteShader;
	Shader* _backgroundShader;
	Shader* _frameBufferShader;

	unsigned int _upscaleBuffer[2];
	unsigned int _upscaleFBO;
	Shader* _upscaleShader;

	unsigned int _finalBuffer;
	unsigned int _finalFBO;

	Shader* _retroShader;

	float _screenWidth;
	float _screenHeight;
	float _lowResWidth;
	float _lowResHeight;
	bool _horizontal;

	Vector2Int _lightPos;

};