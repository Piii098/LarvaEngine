#include "LarvaEngine/Renderer/Renderer2D.h"
#include <gl/glew.h>
#include <algorithm>
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Renderer/VertexArray.h"
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
// �R���X�g���N�^�E�f�X�g���N�^
//==============================================================================

Renderer::Renderer(Game& game)
    : _game(game) {
}

Renderer::~Renderer() {
    Shutdown();
}

//==============================================================================
// �p�u���b�N�֐�
//==============================================================================

// ===== ���������� ===== //

/**
 * OpenGL�A�E�B���h�E�̂̏��������s��
 *
 * �V�F�[�_�[�̃��[�h�Aglew�̏������A�t���[���o�b�t�@�̏��������s��
 */
<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
bool Renderer::Initialize(float screenWidth, float screenHeight, float lowResWidth, float lowResHeight, const std::string& windowName) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _lowResWidth = lowResWidth;
    _lowResHeight = lowResHeight;


=======
bool Renderer2D::Initialize(const std::string& windowName) {

    _windowHeight = _windowHeight == -1 ? 720 : _windowHeight;
    _windowWidth = _windowWidth == -1 ? 1280 : _windowWidth;

    if (_lowResHeight == -1 || _lowResWidth == -1) {
        _lowResHeight = _windowHeight;
        _lowResWidth = _windowWidth;
    }
   
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp
    _numLayers = 16;

    // �p�����b�N�X�W���������� (���ׂ�0.0 = �������C���[�Ɠ�������)
    _parallaxFactors.resize(_numLayers, 0.0f);
    // �f�t�H���g�ł͒����̃��C���[����Ƃ���
    _centralLayer = 10;
    SetParallaxFactor(0, 1.0f);
    SetParallaxFactor(1, 0.95f);
    SetParallaxFactor(2, 0.9f);
    SetParallaxFactor(3, 0.85f);
    SetParallaxFactor(4, 0.8f);
    SetParallaxFactor(5, 0.7f);
    SetParallaxFactor(14, -1.0f);
    SetParallaxFactor(15, 0.f);

    _ambientLightFactors.resize(_numLayers, AmbientLight(Vector3(0.9, 0.8, 1), 0.6));

    // OpenGL�����̐ݒ�
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    _window = SDL_CreateWindow(windowName.c_str(), _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
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
    glGetError(); // GLEW�̏�������ɃG���[���N���A

    glDisable(GL_DITHER);
    glDisable(GL_MULTISAMPLE);

    if (!LoadShaders()) {
        SDL_Log("Failed to load shaders");
        return false;
    }

    // ���_�z��I�u�W�F�N�g���쐬
    CreateVerts();

    if (!InitializeFrameBuffer()) {
        SDL_Log("Failed to initialize FrameBuffer");
        return false;
    }

    return true;
}

// ===== �`�ʏ��� ===== //

/**
<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
 * �Q�[���̕`�ʏ���
 *
 * ��𑜓x�����_�����O�ŃX�v���C�g�A���C�g�̕`��
 * ->����->�u���[���G�t�F�N�g->UI�̕`��->�ŏI�o��
=======
 * ゲームの描写処理
 *
 * 低解像度レンダリングでスプライト、ライトの描写
 * ->合成->ブルームエフェクト->UIの描写->最終出力
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp
 */
void Renderer::Render() {

<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
    // �J�����̃r���[�s����擾
    Camera* camera = _game.GetSceneManager().GetCurrentMainScene().GetCamera();
    Matrix4 view = Matrix4::Identity;
    Vector2Int cameraPos = Vector2Int::Zero;
    if (camera) { // �J���������݂���ꍇ
        view = camera->GetViewMatrix();
        cameraPos = camera->Position();
    }

    // ���C���V�[�����擾
    MainScene& currentMainScene = _game.GetSceneManager().GetCurrentMainScene();

    // �X�e�b�v1: �e���C���[���ƂɃX�v���C�g�ƃ��C�g�̒�𑜓x�����_�����O
=======

    // カメラのビュー行列を取得
    Camera* camera = _game.GetSceneManager().GetCurrentMainScene().GetCamera();
    Matrix4 view = Matrix4::Identity;
    Vector2Int cameraPos = Vector2Int::Zero;
    if (camera) { // カメラが存在する場合
        //SDL_Log("cameraPos, %d,%d", _game.GetSceneManager().GetCurrentMainScene().GetCamera()->Position().x, _game.GetSceneManager().GetCurrentMainScene().GetCamera()->Position().y);
        view = camera->GetViewMatrix();
        cameraPos = camera->Position2D();

        //SDL_Log("Rend CameraPos: %d, %d", cameraPos.x, cameraPos.y);
    }

    // メインシーンを取得
    MainScene& currentMainScene = _game.GetSceneManager().GetCurrentMainScene();

    // ステップ1: 各レイヤーごとにスプライトとライトの低解像度レンダリング
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp
    for (int i = 0; i < _numLayers; i++) {

        glBindFramebuffer(GL_FRAMEBUFFER, _layerFBOs[i]);
        glViewport(0, 0, _lowResWidth, _lowResHeight);
<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
=======
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // ���C���[�ɑΉ�����X�v���C�g��`��
        Matrix4 regionView = view;

        // �J�����ʒu�Ɋ�Â��Ď����I�t�Z�b�g���v�Z
        if (camera) {
            // �p�����b�N�X�W���Ɋ�Â���X����Y���̃I�t�Z�b�g���v�Z
            float parallaxFactor = _parallaxFactors[i];
            Vector3 offset = Vector3::Zero;


            // �p�����b�N�X�W���ɉ����ăJ�����ʒu���王���I�t�Z�b�g���v�Z
            // �W����0�Ȃ璆�����C���[�i�I�t�Z�b�g�Ȃ��j�A1�ɋ߂��قǉ����w�i�Ƃ��ē��������Ȃ��Ȃ�
            offset.x = cameraPos.x * parallaxFactor;
            offset.y = cameraPos.y;

            // �I�t�Z�b�g��K�p�����V�����r���[�s����쐬
            Matrix4 parallaxTranslation = Matrix4::CreateTranslation(offset);
            regionView = regionView * parallaxTranslation;
        }

<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
        // ���C���V�[���̕`��
=======
        // メインシーンの描画
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp
        DrawMainScene(currentMainScene, regionView, i);

        glBindFramebuffer(GL_FRAMEBUFFER, _lightFBOs[i]);
        glViewport(0, 0, _lowResWidth, _lowResHeight);
<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // ���S�ɓ����ȃo�b�N�O���E���h
=======
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);    // 完全に透明なバックグラウンド
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
        // ���C�g�̕`��
=======
        // ライトの描画
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp
        DrawLight(regionView, i);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // �X�e�b�v2: �����p�t���[���o�b�t�@�Ƀ��C���[������
    glBindFramebuffer(GL_FRAMEBUFFER, _combineLightFBO);
    glViewport(0, 0, _lowResWidth, _lowResHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _combineShader->SetActive();
    _frameVerts->SetActive();

    // �e���C���[������
    for (int i = 0; i < _numLayers; i++) {
        CombineLight(i);
    }

<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
    // �X�e�b�v3: �u���[���G�t�F�N�g
=======
    // ステップ3: ブルームエフェクト
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp
    glBindFramebuffer(GL_FRAMEBUFFER, _extractBrightFBO);
    glViewport(0, 0, _lowResWidth, _lowResHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    PostProcess();

<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
    // �X�e�b�v4: UI�����E�B���h�E�T�C�Y�ō��� 
    glBindFramebuffer(GL_FRAMEBUFFER, _finalFBO);
    glViewport(0, 0, _screenWidth, _screenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
=======
    // ステップ4: UIを実ウィンドウサイズで合成 
    glBindFramebuffer(GL_FRAMEBUFFER, _finalFBO);
    glViewport(0, 0, _windowWidth, _windowHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp
    glClear(GL_COLOR_BUFFER_BIT);

    DrawAndCombineUI(currentMainScene);

    // �X�e�b�v3: �ŏI�o�́i�E�B���h�E�ɕ`��j
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
    glViewport(0, 0, _screenWidth, _screenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
=======
    glViewport(0, 0, _windowWidth, _windowHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp
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

// ===== ���C�g�֘A ===== //

/**
 * ���C�g��ǉ�����( ���C���[�͈͓̔��̂ݗL�� )
 */
void Renderer::SetAmbientLightFactor(int layer, AmbientLight factor) {
    if (layer >= 0 && layer < _numLayers) {
        // �����W����0.0�`1.0�͈̔͂ɐ���
        _ambientLightFactors[layer] = factor;
    }
}

/**
 * �S�Ẵ��C���[�̊����W����ݒ肷��( ���C���[�͈͓̔��̂ݗL�� )
 */
void Renderer::SetAllAmbientLightFactor(AmbientLight factor) {
    for (int i = 0; i < _numLayers; i++) {
        SetAmbientLightFactor(i, factor);
    }
}

/**
 * �w�背�C���[�̊����W�����擾����( ���C���[�͈͓̔��̂ݗL�� )
 */
AmbientLight Renderer::GetAmbientLightFactor(int layer) const {
    if (layer >= 0 && layer < _numLayers) {
        return _ambientLightFactors[layer];
    }
    return AmbientLight();
}

/**
 * ���C�g��ǉ�����
 */
void Renderer::AddLight(LightComponent* light) {
    _lights.emplace_back(light);
}

/**
 * ���C�g���폜����
 */
void Renderer::RemoveLight(LightComponent* light) {
    auto iter = std::find(_lights.begin(), _lights.end(), light);
    if (iter != _lights.end()) {
        std::iter_swap(iter, _lights.end() - 1);
        _lights.pop_back();
    }
}


// ===== ���C���[�֘A ===== //

/**
 * �p�����b�N�X�W���̐ݒ�
 *
 * �W����0�𒆉����C���[�Ƃ��A1���Ŕw�ʃ��C���[�Ƃ���
 * -�͑O�ʂɁA+�͔w�ʂɈړ�����
 */
void Renderer::SetParallaxFactor(int layer, float factor) {
    if (layer >= 0 && layer < _numLayers) {
        _parallaxFactors[layer] = factor;
    }
}

/**
 * �p�����b�N�X�W���̎擾
 */
float Renderer::GetParallaxFactor(int layer) const {
    if (layer >= 0 && layer < _numLayers) {
        return _parallaxFactors[layer];
    }
    return 0.0f;
}

/**
 * �������C���[�̐ݒ�
 *
 * �p�����b�N�X�W���̊�ƂȂ郌�C���[��ݒ肷��
 */
void Renderer::SetCentralLayer(int layer) {
    if (layer >= 0 && layer < _numLayers) {
        _centralLayer = layer;
    }
}


//==============================================================================
// �v���C�x�[�g�֐�
//==============================================================================


// ===== ���������� ===== //

/**
 * �t���[���o�b�t�@�̏�����
 */
bool Renderer::InitializeFrameBuffer() {

    // ���C���V�[���p�t���[���o�b�t�@�̍쐬
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


    // ���C�g�p�t���[���o�b�t�@�̍쐬
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

    // �t���[���ƃ��C�g�̍����p�t���[���o�b�t�@�̍쐬
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

    // �u���C�g�l�X���o�p�t���[���o�b�t�@�̍쐬
    glGenFramebuffers(1, &_extractBrightFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _extractBrightFBO);

    glGenTextures(2, _extractBrightBuffers);
    for (unsigned int i = 0; i < 2; i++) {
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

    // �u���[�p�̃t���[���o�b�t�@�̍쐬
    glGenFramebuffers(2, _pingpongFBO);
    glGenTextures(2, _pingpongBuffers);
    for (unsigned int i = 0; i < 2; i++) {
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

    // �u���[���G�t�F�N�g�p�ŏI�t���[���o�b�t�@�̍쐬
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

    // �ŏI�`�ʗp�t���[���o�b�t�@�̍쐬
    glGenFramebuffers(1, &_finalFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _finalFBO);

    glGenTextures(1, &_finalBuffer);
    glBindTexture(GL_TEXTURE_2D, _finalBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _screenWidth, _screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _finalBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SDL_Log("Final framebuffer is not complete!");
        return false;
    }


    // �f�t�H���g�̃t���[���o�b�t�@�ɖ߂�
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}


// ===== �`�ʏ��� ===== //

/**
 * ���C���V�[���̕`�ʏ���
 *
 * ��𑜓x�����_�����O�A�J�����r���[�A�A���r�G���g���C�g�̐ݒ�����A
 * ���݂̃��C���V�[����`�ʂ���
 */
void Renderer::DrawMainScene(MainScene& mainScene, Matrix4 view, int region) {
    _spriteShader->SetActive();
    _spriteVerts->SetActive();

    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_lowResWidth, _lowResHeight);

    _spriteShader->SetMatrixUniform("uViewProj", viewProj);

    _spriteShader->SetMatrixUniform("uViewScreen", view);

    _spriteShader->SetVector3Uniform("ambientLightColor", _ambientLightFactors[region].color);
    _spriteShader->SetFloatUniform("ambientLightIntensity", _ambientLightFactors[region].intensity);

    mainScene.Render(*_spriteShader.get(), region);
}

/**
 * ���C�g�̕`�ʏ���
 *
 * ��𑜓x�����_�����O�A�J�����r���[��ݒ肵
 * ���C�g��`�ʂ���
 */
void Renderer::DrawLight(Matrix4 view, int region) {
    _lightShader->SetActive();
    _spriteVerts->SetActive();

    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_lowResWidth, _lowResHeight);

    _lightShader->SetMatrixUniform("uViewProj", viewProj);

    _lightShader->SetMatrixUniform("uViewScreen", view);

    for (auto& light : _lights) {
        if (light->BufferLayer() == region) {
            light->RenderLight(*_lightShader.get());
        }
    }
}

/**
 * ���C���[�̃X�v���C�g�ƃ��C�g����������
 */
void Renderer::CombineLight(int region) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _layerBuffers[region]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _lightBuffers[region]);
    _combineShader->SetIntUniform("spriteTexture", 0);
    _combineShader->SetIntUniform("lightTexture", 1);
    _combineShader->SetVector3Uniform("uAmbientColor", Vector3(1.f, 1.f, 1.f));
    _combineShader->SetFloatUniform("uAmbientIntensity", 1.f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

/**
 * �|�X�g�v���Z�X����
 *
 * �u���[���G�t�F�N�g��K�p����
 */
void Renderer::PostProcess() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // �u���C�g�l�X���o
    _extractBrightShader->SetActive();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _combineLightBuffer);
    _extractBrightShader->SetIntUniform("frameTexture", 0);

    _frameVerts->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // �u���[����
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

    // �u���[���G�t�F�N�g
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
 * UI�̕`�ʁA��������
 *
 * ���C���V�[���̕`�ʂ��㏑�����AUI��`�ʂ���
 */
void Renderer::DrawAndCombineUI(MainScene& mainScene) {

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
 * UI�̕`�ʏ���
 *
 * ���E�B���h�E�T�C�Y�ɍ��킹��UI��`�ʂ���
 */
void Renderer::DrawUI(MainScene& mainScene) {
    _spriteShader->SetActive();
    _spriteVerts->SetActive();

    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_screenWidth, _screenHeight);

    _spriteShader->SetMatrixUniform("uViewProj", viewProj);
    _spriteShader->SetMatrixUniform("uViewScreen", Matrix4::Identity);

    _spriteShader->SetVector3Uniform("ambientLightColor", Vector3(1.0f, 1.0f, 1.0f));
    _spriteShader->SetFloatUniform("ambientLightIntensity", 1.0f);
    mainScene.RenderUIs(*_spriteShader.get());
}

/**
 * �E�B���h�E�̃X���b�v
 *
 * �_�u���o�b�t�@�����O���s��
 */
void Renderer::SwapWindow() {
    SDL_GL_SwapWindow(_window);
}


// ===== �V�F�[�_�[�֘A ===== //

/**
 * �V�F�[�_�[�̃��[�h
 *
 * �e��V�F�[�_�[�̃��j�t�H�[���ϐ��̐ݒ���s��
 */
bool Renderer::LoadShaders() {
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

    // �V�F�[�_�[�̊�{�ݒ�
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
 * �X�v���C�g�p�̒��_�z����쐬
 *
 * �X�v���C�g�̕`�ʂɎg�p���钸�_�z����쐬����
 */
void Renderer::CreateVerts() {
    // �X�v���C�g�p���_�z��
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

    _spriteVerts = std::make_unique<VertexArray>(spriteVertices, 4, spritIndices, 6);

    // �t���[���o�b�t�@�p���_�z��
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

    _frameVerts = std::make_unique<VertexArray>(frameVertices, 4, frameIndices, 6);
}


// ===== �V���b�g�_�E������ ===== //

/**
 * �V���b�g�_�E������
 *
 * �������̉���AOpenGL�̃R���e�L�X�g�̔j�����s��
 */
<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
void Renderer::Shutdown() {

    if (_frameShader) {
        _frameShader->Unload();
    }

    if (_spriteShader) {
        _spriteShader->Unload();
    }

    if (_lightShader) {
        _lightShader->Unload();
    }

    if (_combineShader) {
        _combineShader->Unload();
    }

    if (_extractBrightShader) {
        _extractBrightShader->Unload();
    }

    if (_blurShader) {
        _blurShader->Unload();
    }

    if (_finalBloomShader) {
        _finalBloomShader->Unload();
    }
=======
void Renderer2D::Shutdown() {
>>>>>>> 544a8b5 (2Dレンダラが出力不可、対処中):src/Renderer/2D/Renderer2D.cpp

    SDL_GL_DestroyContext(_context);
    SDL_DestroyWindow(_window);
}