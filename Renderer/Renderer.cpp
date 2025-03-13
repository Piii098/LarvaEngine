#include "Renderer/Renderer.h"
#include <gl/glew.h>
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Components/Draw/SpriteComponent.h"
#include "Components/Draw/TextComponent.h"
#include "Core/Game.h"
#include "GameObjects/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/UI/UIScene.h"

Renderer::Renderer(Game* game)
    : _game(game)
    , _lowResWidth(0.f)
    , _lowResHeight(0.f)
    , _camera(nullptr)
{
}

Renderer::~Renderer() {
}

#pragma region パブリック関数

bool Renderer::Initialize(float screenWidth, float screenHeight, float lowResWidth, float lowResHeight) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _lowResWidth = lowResWidth;
    _lowResHeight = lowResHeight;

   
    _numLayers = 16;

    // パララックス係数を初期化 (すべて0.0 = 中央レイヤーと同じ動き)
    _parallaxFactors.resize(_numLayers, 0.0f);
    // デフォルトでは中央のレイヤーを基準とする
    _centralLayer = 10;
	SetParallaxFactor(0, 1.0f);
    SetParallaxFactor(1, 0.95f);
    SetParallaxFactor(2, 0.9f);
    SetParallaxFactor(3, 0.85f);
    SetParallaxFactor(4, 0.8f);
    SetParallaxFactor(5, 0.7f);
    SetParallaxFactor(14, -1.0f);
    SetParallaxFactor(15, 0.f);

    // OpenGL属性の設定
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    _window = SDL_CreateWindow("Game", _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
    if (!_window) {
        SDL_Log("Failed to create a window: %s", SDL_GetError());
        return false;
    }

    _context = SDL_GL_CreateContext(_window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        SDL_Log("Failed to initialize GLEW");
        return false;
    }
    glGetError(); // GLEWの初期化後にエラーをクリア

    glDisable(GL_DITHER);
    glDisable(GL_MULTISAMPLE);

    if (!LoadShaders()) {
        SDL_Log("Failed to load shaders");
        return false;
    }

    CreateSpriteVerts();

    if (!InitializeFrameBuffer()) {
        SDL_Log("Failed to initialize FrameBuffer");
        return false;
    }

    return true;
}

void Renderer::Shutdown() {
    delete _frameVerts;
    _frameVerts = nullptr;
    delete _spriteVerts;
    _spriteVerts = nullptr;

    if (_frameShader) {
        _frameShader->Unload();
        delete _frameShader;
    }

    if (_spriteShader) {
        _spriteShader->Unload();
        delete _spriteShader;
    }

    SDL_GL_DestroyContext(_context);
    SDL_DestroyWindow(_window);
}

void Renderer::Render() {

    _camera = _game->GetSceneManager()->GetCurrentMainScene()->GetCamera();
    Matrix4 view;
    Vector2Int cameraPos;

    if (_camera) {
        view = _camera->GetViewMatrix();
        cameraPos = _camera->Position();
    }

	_currentMainScene = _game->GetSceneManager()->GetCurrentMainScene();

    // ステップ1: 各レイヤーを個別にレンダリング
    for (int i = 0; i < _numLayers; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, _layerFBOs[i]);
        glViewport(0, 0, _lowResWidth, _lowResHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // 完全に透明なバックグラウンド
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // レイヤーに対応するスプライトを描画
        Matrix4 regionView = view;

        // カメラ位置に基づいて視差オフセットを計算
        if (_camera) {
            // パララックス係数に基づいてX軸とY軸のオフセットを計算
            float parallaxFactor = _parallaxFactors[i];
            Vector3 offset = Vector3::Zero;


            // パララックス係数に応じてカメラ位置から視差オフセットを計算
            // 係数が0なら中央レイヤー（オフセットなし）、1に近いほど遠い背景として動きが少なくなる
            offset.x = cameraPos.x * parallaxFactor;
            offset.y = cameraPos.y;

            // オフセットを適用した新しいビュー行列を作成
            Matrix4 parallaxTranslation = Matrix4::CreateTranslation(offset);
            regionView = regionView * parallaxTranslation;
        }

        DrawGame(regionView, i);
       
    }

    // ステップ2: 合成用フレームバッファにレイヤーを合成
    glBindFramebuffer(GL_FRAMEBUFFER, _combineUpscaleFBO);
    glViewport(0, 0, _screenWidth, _screenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // レイヤー合成時にアディティブブレンディングを使用
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // アディティブブレンディング

    _frameShader->SetActive();
    _frameVerts->SetActive();

    // 各レイヤーを合成
    for (int i = 0; i < _numLayers; i++) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _layerTextures[i]);
        _frameShader->SetIntUniform("frameTexture", 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    DrawUI();

    // ステップ3: 最終出力（ウィンドウに描画）
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glViewport(0, 0, _screenWidth, _screenHeight);
    glClear(GL_COLOR_BUFFER_BIT);

    // 通常のアルファブレンディングに戻す
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _frameShader->SetActive();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _combineUpscaleBuffer);
    _frameShader->SetIntUniform("frameTexture", 0);

    _frameVerts->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    SwapWindow();
}

// 新しいパララックス関連のメソッド
void Renderer::SetParallaxFactor(int layer, float factor) {
    if (layer >= 0 && layer < _numLayers) {
        // 視差係数を0.0～1.0の範囲に制限
        _parallaxFactors[layer] = factor;
    }
}

float Renderer::GetParallaxFactor(int layer) const {
    if (layer >= 0 && layer < _numLayers) {
        return _parallaxFactors[layer];
    }
    return 0.0f;
}

void Renderer::SetCentralLayer(int layer) {
    if (layer >= 0 && layer < _numLayers) {
        _centralLayer = layer;
    }
}

#pragma endregion


bool Renderer::InitializeFrameBuffer() {
    _layerFBOs.resize(_numLayers);
    _layerTextures.resize(_numLayers);

    // レイヤーごとのフレームバッファの作成
    glGenFramebuffers(_numLayers, _layerFBOs.data());
    glGenTextures(_numLayers, _layerTextures.data());

    for (int i = 0; i < _numLayers; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, _layerFBOs[i]);

        glBindTexture(GL_TEXTURE_2D, _layerTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _lowResWidth, _lowResHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _layerTextures[i], 0);

        // フレームバッファの完全性チェック
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            SDL_Log("Layer framebuffer %d is not complete!", i);
            return false;
        }
    }

    // 合成用フレームバッファの作成
    glGenFramebuffers(1, &_combineUpscaleFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _combineUpscaleFBO);

    glGenTextures(1, &_combineUpscaleBuffer);
    glBindTexture(GL_TEXTURE_2D, _combineUpscaleBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _screenWidth, _screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _combineUpscaleBuffer, 0);

    // 合成フレームバッファの完全性チェック
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SDL_Log("Combine framebuffer is not complete!");
        return false;
    }

    // デフォルトのフレームバッファに戻す
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void Renderer::DrawGame(Matrix4 view, int region) {
    _spriteShader->SetActive();
    _spriteVerts->SetActive();

    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_lowResWidth, _lowResHeight);

    _spriteShader->SetMatrixUniform("uViewProj", viewProj);
    
    _spriteShader->SetMatrixUniform("uViewScreen", view);

    // シンプル化のため、ライティング情報を基本設定のみに
    _spriteShader->SetVector3Uniform("ambientLightColor", Vector3(1.0f, 1.0f, 1.0f));
    _spriteShader->SetFloatUniform("ambientLightIntensity", 1.0f);

    _currentMainScene->Render(_spriteShader, region);
}

void Renderer::DrawUI() {
    _spriteShader->SetActive();
    _spriteVerts->SetActive();

    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_screenWidth, _screenHeight);

    _spriteShader->SetMatrixUniform("uViewProj", viewProj);

    _spriteShader->SetMatrixUniform("uViewScreen", Matrix4::Identity);

    // シンプル化のため、ライティング情報を基本設定のみに
    _spriteShader->SetVector3Uniform("ambientLightColor", Vector3(1.0f, 1.0f, 1.0f));
    _spriteShader->SetFloatUniform("ambientLightIntensity", 1.0f);
    _currentMainScene->RenderUIs(_spriteShader);
}

void Renderer::SwapWindow() {
    SDL_GL_SwapWindow(_window);
}

bool Renderer::LoadShaders() {
    _spriteShader = new Shader();
    if (!_spriteShader->Load("Renderer/Shaders/Sprite.vert", "Renderer/Shaders/Sprite.frag")) {
        SDL_Log("Failed to load SpriteShader");
        return false;
    }

    _frameShader = new Shader();
    if (!_frameShader->Load("Renderer/Shaders/FrameBuffer.vert", "Renderer/Shaders/FrameBuffer.frag")) {
        SDL_Log("Failed to load FrameBuffer shader");
        return false;
    }

    // シェーダーの基本設定
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_lowResWidth, _lowResHeight);

    _spriteShader->SetActive();
    _spriteShader->SetMatrixUniform("uViewProj", viewProj);

    _frameShader->SetActive();
    _frameShader->SetIntUniform("frameTexture", 0);

    return true;
}

void Renderer::CreateSpriteVerts() {
    // スプライト用頂点配列
    float vertices[] = {
        -0.5f,  0.5f, 0.f,  0.f, 0.f, // 左上
         0.5f,  0.5f, 0.f,  1.f, 0.f, // 右上
         0.5f, -0.5f, 0.f,  1.f, 1.f, // 右下
        -0.5f, -0.5f, 0.f,  0.f, 1.f  // 左下
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    _spriteVerts = new VertexArray(vertices, 4, indices, 6);

    // フレームバッファ用頂点配列
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // 左上
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // 右上
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // 右下
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f   // 左下
    };

    unsigned int quadIndices[] = {
        0, 1, 2,  // 最初の三角形
        0, 2, 3   // 二番目の三角形
    };

    _frameVerts = new VertexArray(quadVertices, 4, quadIndices, 6);
}