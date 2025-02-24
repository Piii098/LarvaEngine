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

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void Unload();

	void Draw();

	void AddSprite(SpriteComponent* sprite);
	void RemoveSprite(SpriteComponent* sprite);
	void AddBackground(BGComponent* background);
	void RemoveBackground(BGComponent* background);
	void AddTexture(TextureComponent* texture);
	void RemoveTexture(TextureComponent* texture);

	void SetBrightness(float brightness) { _brightness = brightness; };
	void SetLightPos(Vector2 lightPos) { _lightPos = lightPos; };
private:

	bool InitializeFrameBuffer();

	void DrawBackground();
	void DrawSprite();

	void ApplyBloom();
	bool LoadShaders();
	void CreateSpriteVerts();

	unsigned int _colorBuffer[2];
	unsigned int _hdrFBO;
	VertexArray* _screenVerts;
	Shader* _frameBufferShader;
	bool _isBloom;

	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];

	Shader* _blurShader;               // ガウシアンブラー用シェーダー
	Shader* _bloomFinalShader;         // 最終合成用シェーダー

	// ブルーム設定
	float _bloomThreshold = 1.0f;      // 明るい部分とみなす閾値
	float _bloomIntensity = 0.8f;      // ブルームの強度

	std::vector<BGComponent*> _backgrounds;
	std::vector<SpriteComponent*> _sprites;
	std::vector<TextureComponent*> _textures;
	std::vector<Line*> _lines;

	Game* _game;

	SDL_Window* _window; //ウィンドウ
	SDL_GLContext _context;
	VertexArray* _spriteVerts;
	Shader* _spriteShader;
	Shader* _backgroundShader;

	float _screenWidth;
	float _screenHeight;

	float _brightness;
	Vector2 _lightPos;

	std::vector<SpriteComponent*> _debugBoxes;

};