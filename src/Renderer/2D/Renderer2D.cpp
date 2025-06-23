#include "LarvaEngine/Renderer/2D/Renderer2D.h"
#include <gl/glew.h>
#include <algorithm>
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Renderer/2D/VertexArray2D.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/GameObjects/Camera.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/UIScene.h"
#include "LarvaEngine/Components/Light/LightComponent.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

Renderer2D::Renderer2D(Game& game) 
	: Renderer(game){
}

Renderer2D::~Renderer2D() {
	Shutdown();
}

//==============================================================================
// パブリック関数
//==============================================================================

// ===== 初期化処理 ===== //

/**
 * OpenGL、ウィンドウのの初期化を行う
 * 
 * シェーダーのロード、glewの初期化、フレームバッファの初期化を行う
 */
bool Renderer2D::Initialize(const std::string& windowName) {

    _windowHeight = _windowHeight == -1 ? 720 : _windowHeight;
    _windowWidth = _windowWidth == -1 ? 1280 : _windowWidth;

    if (_lowResHeight == -1 || _lowResWidth == -1) {
        _lowResHeight = _windowHeight;
        _lowResWidth = _windowWidth;
    }
   
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

    _ambientLightFactors.resize(_numLayers, AmbientLight(Vector3(0.9,0.8,1), 0.6));

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

    _window = SDL_CreateWindow(windowName.c_str(), _windowWidth, _windowHeight, SDL_WINDOW_OPENGL);
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

	// 頂点配列オブジェクトを作成
    CreateVerts();

    if (!InitializeFrameBuffer()) {
        SDL_Log("Failed to initialize FrameBuffer");
        return false;
    }

    return true;
}

// ===== 描写処理 ===== //

/**
 * ゲームの描写処理
 *
 * 低解像度レンダリングでスプライト、ライトの描写
 * ->合成->ブルームエフェクト->UIの描写->最終出力
 */
void Renderer2D::Render() {


    // カメラのビュー行列を取得
    Camera* camera = _game.GetSceneManager().GetCurrentMainScene().GetCamera();
    Matrix4 view = Matrix4::Identity;
    Vector2 cameraPos = Vector2::Zero;
    if (camera) { // カメラが存在する場合
        //SDL_Log("cameraPos, %d,%d", _game.GetSceneManager().GetCurrentMainScene().GetCamera()->Position().x, _game.GetSceneManager().GetCurrentMainScene().GetCamera()->Position().y);
        view = camera->GetViewMatrix();
        cameraPos = camera->Position2D();

        //SDL_Log("Rend CameraPos: %d, %d", cameraPos.x, cameraPos.y);
    }

    // メインシーンを取得
    MainScene& currentMainScene = _game.GetSceneManager().GetCurrentMainScene();

    // ステップ1: 各レイヤーごとにスプライトとライトの低解像度レンダリング
    for (int i = 0; i < _numLayers; i++) {

        glBindFramebuffer(GL_FRAMEBUFFER, _layerFBOs[i]);
        glViewport(0, 0, _lowResWidth, _lowResHeight);
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // レイヤーに対応するスプライトを描画
        Matrix4 regionView = view;

        // カメラ位置に基づいて視差オフセットを計算
        if (camera) {
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

        // メインシーンの描画
        DrawMainScene(currentMainScene, regionView, i);

        glBindFramebuffer(GL_FRAMEBUFFER, _lightFBOs[i]);
        glViewport(0, 0, _lowResWidth, _lowResHeight);
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);    // 完全に透明なバックグラウンド
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        // ライトの描画
        DrawLight(regionView, i);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ステップ2: 合成用フレームバッファにレイヤーを合成
    glBindFramebuffer(GL_FRAMEBUFFER, _combineLightFBO);
    glViewport(0, 0, _lowResWidth, _lowResHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _combineShader->SetActive();
    _frameVerts->SetActive();

    // 各レイヤーを合成
    for (int i = 0; i < _numLayers; i++) {
        CombineLight(i);
    }

    // ステップ3: ブルームエフェクト
    glBindFramebuffer(GL_FRAMEBUFFER, _extractBrightFBO);
    glViewport(0, 0, _lowResWidth, _lowResHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    PostProcess();

    // ステップ4: UIを実ウィンドウサイズで合成 
    glBindFramebuffer(GL_FRAMEBUFFER, _finalFBO);
    glViewport(0, 0, _windowWidth, _windowHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    DrawAndCombineUI(currentMainScene);

    // ステップ3: 最終出力（ウィンドウに描画）
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, _windowWidth, _windowHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _frameShader->SetActive();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _finalBuffer);
    _frameShader->SetIntUniform("frameTexture", 0);

    _frameVerts->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    SwapWindow();



    /*

    Matrix4 view = Matrix4::Identity;

    // メインシーンを取得
    MainScene& currentMainScene = _game.GetSceneManager().GetCurrentMainScene();
    // デフォルトフレームバッファに描画することを明確にする
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, _windowWidth, _windowHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    _spriteShader->SetActive();
    _spriteVerts->SetActive();

    if (!_spriteVerts) {
        SDL_Log("ERROR: _spriteVerts is null!");
        return;
    }

    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_windowWidth, _windowHeight);

    _spriteShader->SetMatrixUniform("uViewProj", view * viewProj);
    _spriteShader->SetMatrixUniform("uWorldTransform", Matrix4::Identity);
    _spriteShader->SetIntUniform("uTexture", 0);
    Texture* sample = _game.GetTextureManager().Get("Sample");
    sample->SetActive();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    SwapWindow();

    */
}

// ===== ライト関連 ===== //

/**
 * ライトを追加する( レイヤーの範囲内のみ有効 )
 */
void Renderer2D::SetAmbientLightFactor(int layer, AmbientLight factor) {
    if (layer >= 0 && layer < _numLayers) {
        // 環境光係数を0.0～1.0の範囲に制限
        _ambientLightFactors[layer] = factor;
    }
}

/**
 * 全てのレイヤーの環境光係数を設定する( レイヤーの範囲内のみ有効 )
 */
void Renderer2D::SetAllAmbientLightFactor(AmbientLight factor) {
    for (int i = 0; i < _numLayers; i++) {
        SetAmbientLightFactor(i, factor);
    }
}

/**
 * 指定レイヤーの環境光係数を取得する( レイヤーの範囲内のみ有効 )
 */
AmbientLight Renderer2D::GetAmbientLightFactor(int layer) const {
    if (layer >= 0 && layer < _numLayers) {
        return _ambientLightFactors[layer];
    }
    return AmbientLight();
}

/**
 * ライトを追加する
 */
void Renderer2D::AddLight(LightComponent* light) {
    _lights.emplace_back(light);
}

/**
 * ライトを削除する
 */
void Renderer2D::RemoveLight(LightComponent* light) {
    auto iter = std::find(_lights.begin(), _lights.end(), light);
    if (iter != _lights.end()) {
        std::iter_swap(iter, _lights.end() - 1);
        _lights.pop_back();
    }
}


// ===== レイヤー関連 ===== //

/**
 * パララックス係数の設定
 * 
 * 係数は0を中央レイヤーとし、1を最背面レイヤーとする
 * -は前面に、+は背面に移動する
 */
void Renderer2D::SetParallaxFactor(int layer, float factor) {
    if (layer >= 0 && layer < _numLayers) {
        _parallaxFactors[layer] = factor;
    }
}

/**
 * パララックス係数の取得
 */
float Renderer2D::GetParallaxFactor(int layer) const {
    if (layer >= 0 && layer < _numLayers) { 
        return _parallaxFactors[layer];
    }
    return 0.0f;
}

/**
 * 中央レイヤーの設定
 * 
 * パララックス係数の基準となるレイヤーを設定する
 */
void Renderer2D::SetCentralLayer(int layer) {
    if (layer >= 0 && layer < _numLayers) {
        _centralLayer = layer;
    }
}


//==============================================================================
// プライベート関数
//==============================================================================


// ===== 初期化処理 ===== //

/**
 * フレームバッファの初期化
 */
bool Renderer2D::InitializeFrameBuffer() {

	// メインシーン用フレームバッファの作成
    _layerFBOs.resize(_numLayers);
    _layerBuffers.resize(_numLayers);

    glGenFramebuffers(_numLayers, _layerFBOs.data());
    glGenTextures(_numLayers, _layerBuffers.data());

    for (int i = 0; i < _numLayers; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, _layerFBOs[i]);

        glBindTexture(GL_TEXTURE_2D, _layerBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _lowResWidth, _lowResHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _layerBuffers[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            SDL_Log("Layer framebuffer %d is not complete!", i);
            return false;
        }
    }

	_lightFBOs.resize(_numLayers);
    _lightBuffers.resize(_numLayers);


	// ライト用フレームバッファの作成
    glGenFramebuffers(_numLayers, _lightFBOs.data());
    glGenTextures(_numLayers, _lightBuffers.data());

    for (int i = 0; i < _numLayers; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, _lightFBOs[i]);

        glBindTexture(GL_TEXTURE_2D, _lightBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _lowResWidth, _lowResHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _lightBuffers[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            SDL_Log("Layer framebuffer %d is not complete!", i);
            return false;
        }
    }

	// フレームとライトの合成用フレームバッファの作成
    glGenFramebuffers(1, &_combineLightFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _combineLightFBO);

    glGenTextures(1, &_combineLightBuffer);
    glBindTexture(GL_TEXTURE_2D, _combineLightBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _lowResWidth, _lowResHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _combineLightBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SDL_Log("Combine framebuffer is not complete!");
        return false;
    }
    
	// ブライトネス抽出用フレームバッファの作成
    glGenFramebuffers(1, &_extractBrightFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _extractBrightFBO);

    glGenTextures(2, _extractBrightBuffers);
    for (unsigned int i = 0; i < 2; i++){
        glBindTexture(GL_TEXTURE_2D, _extractBrightBuffers[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA16F, _lowResWidth, _lowResHeight, 0, GL_RGBA, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _extractBrightBuffers[i], 0);
    }

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

	// ブラー用のフレームバッファの作成
    glGenFramebuffers(2, _pingpongFBO);
    glGenTextures(2, _pingpongBuffers);
    for (unsigned int i = 0; i < 2; i++){
        glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, _pingpongBuffers[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA16F, _lowResWidth, _lowResHeight, 0, GL_RGBA, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pingpongBuffers[i], 0);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            SDL_Log("Blur framebuffer is not complete!");
            return false;
        }
    }

	// ブルームエフェクト用最終フレームバッファの作成
    glGenFramebuffers(1, &_finalBloomFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _finalBloomFBO);

    glGenTextures(1, &_finalBloomBuffer);
    glBindTexture(GL_TEXTURE_2D, _finalBloomBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _lowResWidth, _lowResHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _finalBloomBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SDL_Log("Final Bloom framebuffer is not complete!");
        return false;
    }

	// 最終描写用フレームバッファの作成
    glGenFramebuffers(1, &_finalFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _finalFBO);

    glGenTextures(1, &_finalBuffer);
    glBindTexture(GL_TEXTURE_2D, _finalBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _windowWidth, _windowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _finalBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SDL_Log("Final framebuffer is not complete!");
        return false;
    }


    // デフォルトのフレームバッファに戻す
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}


// ===== 描写処理 ===== //

/**
 * メインシーンの描写処理
 * 
 * 低解像度レンダリング、カメラビュー、アンビエントライトの設定をし、
 * 現在のメインシーンを描写する
 */
void Renderer2D::DrawMainScene(MainScene& mainScene, Matrix4 view, int region) {
    _spriteShader->SetActive();
    _spriteVerts->SetActive();

    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_lowResWidth, _lowResHeight);

    _spriteShader->SetMatrixUniform("uViewProj", viewProj);
    
    _spriteShader->SetMatrixUniform("uViewCamera", view);

	_spriteShader->SetVector3Uniform("ambientLightColor", _ambientLightFactors[region].color);
	_spriteShader->SetFloatUniform("ambientLightIntensity", _ambientLightFactors[region].intensity);

    mainScene.RenderSprite(*_spriteShader.get(), region);
}

/**
 * ライトの描写処理
 * 
 * 低解像度レンダリング、カメラビューを設定し
 * ライトを描写する
 */
void Renderer2D::DrawLight(Matrix4 view, int region) {
    _lightShader->SetActive();
    _spriteVerts->SetActive();

    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_lowResWidth, _lowResHeight);

    _lightShader->SetMatrixUniform("uViewProj", viewProj);

    _lightShader->SetMatrixUniform("uViewCamera", view);

    for (auto& light : _lights) {
        if (light->BufferLayer() == region) {
            light->RenderLight(*_lightShader.get());
        }
    }
}

/**
 * レイヤーのスプライトとライトを合成する
 */
void Renderer2D::CombineLight(int region) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _layerBuffers[region]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _lightBuffers[region]);
    _combineShader->SetIntUniform("spriteTexture", 0);
    _combineShader->SetIntUniform("lightTexture", 1);
    _combineShader->SetVector3Uniform("uAmbientColor", Vector3(1.f,1.f,1.f));
	_combineShader->SetFloatUniform("uAmbientIntensity", 1.f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

/**
 * ポストプロセス処理
 * 
 * ブルームエフェクトを適用する
 */
void Renderer2D::PostProcess() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	// ブライトネス抽出
    _extractBrightShader->SetActive();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _combineLightBuffer);
    _extractBrightShader->SetIntUniform("frameTexture", 0);

    _frameVerts->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// ブラー処理
    bool horizontal = true, first_iteration = true;
    int amount = 5;

    _blurShader->SetActive();

    for (unsigned int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[horizontal]);
        _blurShader->SetIntUniform("horizontal", horizontal);
        glBindTexture(
            GL_TEXTURE_2D, first_iteration ? _extractBrightBuffers[1] : _pingpongBuffers[!horizontal]
        );
        _frameVerts->SetActive();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }

	// ブルームエフェクト
    glBindFramebuffer(GL_FRAMEBUFFER, _finalBloomFBO);
    _finalBloomShader->SetActive();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _extractBrightBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _pingpongBuffers[!horizontal]);
    _finalBloomShader->SetIntUniform("scene", 0);
    _finalBloomShader->SetIntUniform("bloomBlur", 1);
    _finalBloomShader->SetFloatUniform("exposure", 1.0f);
    _finalBloomShader->SetIntUniform("uIsBloom", true);

    _frameVerts->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}

/**
 * UIの描写、合成処理
 *
 * メインシーンの描写を上書きし、UIを描写する
 */
void Renderer2D::DrawAndCombineUI(MainScene& mainScene) {

    _frameShader->SetActive();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _finalBloomBuffer);
    _frameShader->SetIntUniform("frameTexture", 0);
	_frameShader->SetFloatUniform("uAmbientFactor", 1.0f); 

    _frameVerts->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    DrawUI(mainScene);
}

/**
 * UIの描写処理
 *
 * 実ウィンドウサイズに合わせてUIを描写する
 */
void Renderer2D::DrawUI(MainScene& mainScene) {
    _spriteShader->SetActive();
    _spriteVerts->SetActive();

    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_windowWidth, _windowHeight);

    _spriteShader->SetMatrixUniform("uViewProj", viewProj);
    _spriteShader->SetMatrixUniform("uViewCamera", Matrix4::Identity);

    _spriteShader->SetVector3Uniform("ambientLightColor", Vector3(1.0f, 1.0f, 1.0f));
    _spriteShader->SetFloatUniform("ambientLightIntensity", 1.0f);
    mainScene.RenderUIs(*_spriteShader.get());
}

/**
 * ウィンドウのスワップ
 * 
 * ダブルバッファリングを行う
 */
void Renderer2D::SwapWindow() {
    SDL_GL_SwapWindow(_window);
}


// ===== シェーダー関連 ===== //

/**
 * シェーダーのロード
 * 
 * 各種シェーダーのユニフォーム変数の設定を行う
 */
bool Renderer2D::LoadShaders() {
    _spriteShader = std::make_unique<Shader>();
    if (!_spriteShader->Load("Assets/Shaders/Sprite.vert", "Assets/Shaders/Sprite.frag")) {
        SDL_Log("Failed to load SpriteShader");
        return false;
    }

    _frameShader = std::make_unique<Shader>();
    if (!_frameShader->Load("Assets/Shaders/FrameBuffer.vert", "Assets/Shaders/FrameBuffer.frag")) {
        SDL_Log("Failed to load FrameBuffer shader");
        return false;
    }

	_lightShader = std::make_unique<Shader>();
    if (!_lightShader->Load("Assets/Shaders/Light.vert", "Assets/Shaders/Light.frag")) {
		SDL_Log("Failed to load Light shader");
		return false;
    }

	_combineShader = std::make_unique<Shader>();
    if (!_combineShader->Load("Assets/Shaders/FrameBuffer.vert", "Assets/Shaders/Combine.frag")) {
		SDL_Log("Failed to load Combine shader");
		return false;
    }

	_extractBrightShader = std::make_unique<Shader>();
    if (!_extractBrightShader->Load("Assets/Shaders/FrameBuffer.vert", "Assets/Shaders/ExtractBright.frag")) {
		SDL_Log("Failed to load ExtractBright shader");
		return false;
    }

	_blurShader = std::make_unique<Shader>();
    if (!_blurShader->Load("Assets/Shaders/FrameBuffer.vert", "Assets/Shaders/Blur.frag")) {
		SDL_Log("Failed to load Blur shader");
		return false;
    }

	_finalBloomShader = std::make_unique<Shader>();
    if (!_finalBloomShader->Load("Assets/Shaders/FrameBuffer.vert", "Assets/Shaders/FinalBloom.frag")) {
		SDL_Log("Failed to load FinalBloom shader");
		return false;
    }

    // シェーダーの基本設定
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_lowResWidth, _lowResHeight);

    _spriteShader->SetActive();
    _spriteShader->SetMatrixUniform("uViewProj", viewProj);

    _lightShader->SetActive();
	_lightShader->SetMatrixUniform("uViewProj", viewProj);

    _frameShader->SetActive();
    _frameShader->SetIntUniform("frameTexture", 0);

	_combineShader->SetActive();
	_combineShader->SetIntUniform("spriteTexture", 0);
	_combineShader->SetIntUniform("lightTexture", 1);

	_extractBrightShader->SetActive();
	_extractBrightShader->SetIntUniform("frameTexture", 0);

	_blurShader->SetActive();
	_blurShader->SetIntUniform("image", 0);

	_finalBloomShader->SetActive();
	_finalBloomShader->SetIntUniform("scene", 0);
	_finalBloomShader->SetIntUniform("bloomBlur", 1);

    return true;
}

/**
 * スプライト用の頂点配列を作成
 * 
 * スプライトの描写に使用する頂点配列を作成する
 */
void Renderer2D::CreateVerts() {
    // スプライト用頂点配列
    float spriteVertices[] = {
        -0.5f,  0.5f, 0.f,  0.f, 0.f, 
         0.5f,  0.5f, 0.f,  1.f, 0.f, 
         0.5f, -0.5f, 0.f,  1.f, 1.f,
        -0.5f, -0.5f, 0.f,  0.f, 1.f 
    };

    unsigned int spritIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    _spriteVerts = std::make_unique<VertexArray2D>(spriteVertices, 4, spritIndices, 6);

    // フレームバッファ用頂点配列
    float frameVertices[] = {
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f 
    };

    unsigned int frameIndices[] = {
        0, 1, 2, 
        0, 2, 3   
    };

	_frameVerts = std::make_unique<VertexArray2D>(frameVertices, 4, frameIndices, 6);
}


// ===== シャットダウン処理 ===== //

/**
 * シャットダウン処理
 *
 * メモリの解放、OpenGLのコンテキストの破棄を行う
 */
void Renderer2D::Shutdown() {

    SDL_GL_DestroyContext(_context);
    SDL_DestroyWindow(_window);
}