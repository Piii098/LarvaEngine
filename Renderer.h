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
	void SetBrightness(float brightness) { _brightness = brightness; };
	void SetLightPos(Vector2 lightPos) { _lightPos = lightPos; };
private:

	bool InitializeFrameBuffer();

	void ApplyBloom();
	bool LoadShaders();
	void CreateSpriteVerts();

	unsigned int framebuffer;
	unsigned int colorBuffers[2];
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];
	Shader* bloomShader;

	std::unordered_map<std::string, Texture*> _textures;
	std::vector<SpriteComponent*> _sprites;
	std::vector<Line*> _lines;

	Game* _game;

	SDL_Window* _window; //ウィンドウ
	SDL_GLContext _context;
	VertexArray* _spriteVerts;
	Shader* _spriteShader;

	float _screenWidth;
	float _screenHeight;

	float _brightness;
	Vector2 _lightPos;

	std::vector<SpriteComponent*> _debugBoxes;

};