#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "Utilities/Math.h"

class Game;
class VertexArray;
class Shader;
class Camera;
class SpriteComponent;
class TextComponent;
class MainScene;
class UIScene;

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
    void SetCentralLayer(int layer);

    void UnloadData() {};

private:

    bool InitializeFrameBuffer();
    void DrawGame(Matrix4 view, int region);
	void DrawUI();
    void SwapWindow();
    bool LoadShaders();
    void CreateSpriteVerts();

    // フレームバッファ関連
    std::vector<unsigned int> _layerFBOs;
    std::vector<unsigned int> _layerTextures;
    unsigned int _combineUpscaleFBO;
    unsigned int _combineUpscaleBuffer;
    int _numLayers;

    // パララックス関連
    std::vector<float> _parallaxFactors;  // 各レイヤーの視差係数 (0.0=中央レイヤーと同じ、1.0=最背面)
    int _centralLayer;                    // 中央レイヤー (基準となるレイヤー)

    // 基本的なレンダリングリソース
    SDL_Window* _window;
    SDL_GLContext _context;
    VertexArray* _frameVerts;
    VertexArray* _spriteVerts;
    Shader* _frameShader;
    Shader* _spriteShader;

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
	//std::vector<UIScreen*> _uiScreens;

};