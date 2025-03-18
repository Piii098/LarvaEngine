#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "Utilities/Math.h"

struct AmbientLight {
    AmbientLight(Vector3 color, float intensity) : color(color), intensity(intensity) {}
    AmbientLight() : color(Vector3(1, 1, 1)), intensity(1.f) {}
    Vector3 color;
	float intensity;
};

class Game;
class VertexArray;
class Shader;
class Camera;
class SpriteComponent;
class TextComponent;
class MainScene;
class UIScene;
class LightComponent;

class Renderer {
public:
    Renderer(Game* game);
    ~Renderer();

    bool Initialize(float screenWidth, float screenHeight, float lowResWidth, float lowResHeight);
    void Shutdown();
    void Render();

    // スプライト管理機能
   
    void SetParallaxFactor(int layer, float factor);
    float GetParallaxFactor(int layer) const;
	void SetAmbientLightFactor(int layer, AmbientLight factor);
	void SetAllAmbientLightFactor(AmbientLight factor);
    AmbientLight GetAmbientLightFactor(int layer) const;
    void SetCentralLayer(int layer);

	void AddLight(LightComponent* light);
	void RemoveLight(LightComponent* light);

    void UnloadData() {};

private:

    bool InitializeFrameBuffer();
    void DrawGame(Matrix4 view, int region);
	void DrawLight(Matrix4 view, int region);
	void CombineLight(int region);
	void CombineUI();
	void DrawUI();
    void SwapWindow();
    bool LoadShaders();
    void CreateSpriteVerts();

    // フレームバッファ関連
    std::vector<unsigned int> _layerFBOs;
    std::vector<unsigned int> _layerTextures;
	std::vector<unsigned int> _lightFBOs;
	std::vector<unsigned int> _lightTextures;
	//post processing
	unsigned int _extractBrightFBO;
	unsigned int _extractBrightBuffer[2];
	unsigned int _pingpongFBO[2];
	unsigned int _pingpongBuffer[2];
	unsigned int _finalBloomFBO;
	unsigned int _finalBloomBuffer;

    unsigned int _combineLightFBO;
    unsigned int _combineLightBuffer;
	unsigned int _finalFBO;
	unsigned int _finalBuffer;
    int _numLayers;

    // パララックス関連
    std::vector<float> _parallaxFactors;  // 各レイヤーの視差係数 (0.0=中央レイヤーと同じ、1.0=最背面)
    int _centralLayer;                    // 中央レイヤー (基準となるレイヤー)

    //
	std::vector<AmbientLight> _ambientLightFactors;


    // 基本的なレンダリングリソース
    SDL_Window* _window;
    SDL_GLContext _context;
    VertexArray* _frameVerts;
    VertexArray* _spriteVerts;
    Shader* _frameShader;
    Shader* _spriteShader;
	Shader* _lightShader;
	Shader* _combineShader;
	Shader* _extractBrightShader;
	Shader* _blurShader;
    Shader* _finalBloomShader;

    // ウィンドウ設定
    float _screenWidth;
    float _screenHeight;
    float _lowResWidth;
    float _lowResHeight;

    // ゲームリソース
    Game* _game;
    MainScene* _currentMainScene;
	UIScene* _uiScene;
    Camera* _camera;
	std::vector<LightComponent*> _lights;
	//std::vector<UIScreen*> _uiScreens;

};