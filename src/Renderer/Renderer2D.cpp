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
// ƒRƒ“ƒXƒgƒ‰ƒNƒ^EƒfƒXƒgƒ‰ƒNƒ^
//==============================================================================

Renderer::Renderer(Game& game)
    : _game(game) {
}

Renderer::~Renderer() {
    Shutdown();
}

//==============================================================================
// ƒpƒuƒŠƒbƒNŠÖ”
//==============================================================================

// ===== ‰Šú‰»ˆ— ===== //

/**
 * OpenGLAƒEƒBƒ“ƒhƒE‚Ì‚Ì‰Šú‰»‚ğs‚¤
 *
 * ƒVƒF[ƒ_[‚Ìƒ[ƒhAglew‚Ì‰Šú‰»AƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚Ì‰Šú‰»‚ğs‚¤
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
   
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp
    _numLayers = 16;

    // ƒpƒ‰ƒ‰ƒbƒNƒXŒW”‚ğ‰Šú‰» (‚·‚×‚Ä0.0 = ’†‰›ƒŒƒCƒ„[‚Æ“¯‚¶“®‚«)
    _parallaxFactors.resize(_numLayers, 0.0f);
    // ƒfƒtƒHƒ‹ƒg‚Å‚Í’†‰›‚ÌƒŒƒCƒ„[‚ğŠî€‚Æ‚·‚é
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

    // OpenGL‘®«‚Ìİ’è
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
    glGetError(); // GLEW‚Ì‰Šú‰»Œã‚ÉƒGƒ‰[‚ğƒNƒŠƒA

    glDisable(GL_DITHER);
    glDisable(GL_MULTISAMPLE);

    if (!LoadShaders()) {
        SDL_Log("Failed to load shaders");
        return false;
    }

    // ’¸“_”z—ñƒIƒuƒWƒFƒNƒg‚ğì¬
    CreateVerts();

    if (!InitializeFrameBuffer()) {
        SDL_Log("Failed to initialize FrameBuffer");
        return false;
    }

    return true;
}

// ===== •`Êˆ— ===== //

/**
<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
 * ƒQ[ƒ€‚Ì•`Êˆ—
 *
 * ’á‰ğ‘œ“xƒŒƒ“ƒ_ƒŠƒ“ƒO‚ÅƒXƒvƒ‰ƒCƒgAƒ‰ƒCƒg‚Ì•`Ê
 * ->‡¬->ƒuƒ‹[ƒ€ƒGƒtƒFƒNƒg->UI‚Ì•`Ê->ÅIo—Í
=======
 * ã‚²ãƒ¼ãƒ ã®æå†™å‡¦ç†
 *
 * ä½è§£åƒåº¦ãƒ¬ãƒ³ãƒ€ãƒªãƒ³ã‚°ã§ã‚¹ãƒ—ãƒ©ã‚¤ãƒˆã€ãƒ©ã‚¤ãƒˆã®æå†™
 * ->åˆæˆ->ãƒ–ãƒ«ãƒ¼ãƒ ã‚¨ãƒ•ã‚§ã‚¯ãƒˆ->UIã®æå†™->æœ€çµ‚å‡ºåŠ›
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp
 */
void Renderer::Render() {

<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
    // ƒJƒƒ‰‚Ìƒrƒ…[s—ñ‚ğæ“¾
    Camera* camera = _game.GetSceneManager().GetCurrentMainScene().GetCamera();
    Matrix4 view = Matrix4::Identity;
    Vector2Int cameraPos = Vector2Int::Zero;
    if (camera) { // ƒJƒƒ‰‚ª‘¶İ‚·‚éê‡
        view = camera->GetViewMatrix();
        cameraPos = camera->Position();
    }

    // ƒƒCƒ“ƒV[ƒ“‚ğæ“¾
    MainScene& currentMainScene = _game.GetSceneManager().GetCurrentMainScene();

    // ƒXƒeƒbƒv1: ŠeƒŒƒCƒ„[‚²‚Æ‚ÉƒXƒvƒ‰ƒCƒg‚Æƒ‰ƒCƒg‚Ì’á‰ğ‘œ“xƒŒƒ“ƒ_ƒŠƒ“ƒO
=======

    // ã‚«ãƒ¡ãƒ©ã®ãƒ“ãƒ¥ãƒ¼è¡Œåˆ—ã‚’å–å¾—
    Camera* camera = _game.GetSceneManager().GetCurrentMainScene().GetCamera();
    Matrix4 view = Matrix4::Identity;
    Vector2Int cameraPos = Vector2Int::Zero;
    if (camera) { // ã‚«ãƒ¡ãƒ©ãŒå­˜åœ¨ã™ã‚‹å ´åˆ
        //SDL_Log("cameraPos, %d,%d", _game.GetSceneManager().GetCurrentMainScene().GetCamera()->Position().x, _game.GetSceneManager().GetCurrentMainScene().GetCamera()->Position().y);
        view = camera->GetViewMatrix();
        cameraPos = camera->Position2D();

        //SDL_Log("Rend CameraPos: %d, %d", cameraPos.x, cameraPos.y);
    }

    // ãƒ¡ã‚¤ãƒ³ã‚·ãƒ¼ãƒ³ã‚’å–å¾—
    MainScene& currentMainScene = _game.GetSceneManager().GetCurrentMainScene();

    // ã‚¹ãƒ†ãƒƒãƒ—1: å„ãƒ¬ã‚¤ãƒ¤ãƒ¼ã”ã¨ã«ã‚¹ãƒ—ãƒ©ã‚¤ãƒˆã¨ãƒ©ã‚¤ãƒˆã®ä½è§£åƒåº¦ãƒ¬ãƒ³ãƒ€ãƒªãƒ³ã‚°
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp
    for (int i = 0; i < _numLayers; i++) {

        glBindFramebuffer(GL_FRAMEBUFFER, _layerFBOs[i]);
        glViewport(0, 0, _lowResWidth, _lowResHeight);
<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
=======
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // ƒŒƒCƒ„[‚É‘Î‰‚·‚éƒXƒvƒ‰ƒCƒg‚ğ•`‰æ
        Matrix4 regionView = view;

        // ƒJƒƒ‰ˆÊ’u‚ÉŠî‚Ã‚¢‚Ä‹·ƒIƒtƒZƒbƒg‚ğŒvZ
        if (camera) {
            // ƒpƒ‰ƒ‰ƒbƒNƒXŒW”‚ÉŠî‚Ã‚¢‚ÄX²‚ÆY²‚ÌƒIƒtƒZƒbƒg‚ğŒvZ
            float parallaxFactor = _parallaxFactors[i];
            Vector3 offset = Vector3::Zero;


            // ƒpƒ‰ƒ‰ƒbƒNƒXŒW”‚É‰‚¶‚ÄƒJƒƒ‰ˆÊ’u‚©‚ç‹·ƒIƒtƒZƒbƒg‚ğŒvZ
            // ŒW”‚ª0‚È‚ç’†‰›ƒŒƒCƒ„[iƒIƒtƒZƒbƒg‚È‚µjA1‚É‹ß‚¢‚Ù‚Ç‰“‚¢”wŒi‚Æ‚µ‚Ä“®‚«‚ª­‚È‚­‚È‚é
            offset.x = cameraPos.x * parallaxFactor;
            offset.y = cameraPos.y;

            // ƒIƒtƒZƒbƒg‚ğ“K—p‚µ‚½V‚µ‚¢ƒrƒ…[s—ñ‚ğì¬
            Matrix4 parallaxTranslation = Matrix4::CreateTranslation(offset);
            regionView = regionView * parallaxTranslation;
        }

<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
        // ƒƒCƒ“ƒV[ƒ“‚Ì•`‰æ
=======
        // ãƒ¡ã‚¤ãƒ³ã‚·ãƒ¼ãƒ³ã®æç”»
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp
        DrawMainScene(currentMainScene, regionView, i);

        glBindFramebuffer(GL_FRAMEBUFFER, _lightFBOs[i]);
        glViewport(0, 0, _lowResWidth, _lowResHeight);
<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Š®‘S‚É“§–¾‚ÈƒoƒbƒNƒOƒ‰ƒEƒ“ƒh
=======
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);    // å®Œå…¨ã«é€æ˜ãªãƒãƒƒã‚¯ã‚°ãƒ©ã‚¦ãƒ³ãƒ‰
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
        // ƒ‰ƒCƒg‚Ì•`‰æ
=======
        // ãƒ©ã‚¤ãƒˆã®æç”»
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp
        DrawLight(regionView, i);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ƒXƒeƒbƒv2: ‡¬—pƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚ÉƒŒƒCƒ„[‚ğ‡¬
    glBindFramebuffer(GL_FRAMEBUFFER, _combineLightFBO);
    glViewport(0, 0, _lowResWidth, _lowResHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _combineShader->SetActive();
    _frameVerts->SetActive();

    // ŠeƒŒƒCƒ„[‚ğ‡¬
    for (int i = 0; i < _numLayers; i++) {
        CombineLight(i);
    }

<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
    // ƒXƒeƒbƒv3: ƒuƒ‹[ƒ€ƒGƒtƒFƒNƒg
=======
    // ã‚¹ãƒ†ãƒƒãƒ—3: ãƒ–ãƒ«ãƒ¼ãƒ ã‚¨ãƒ•ã‚§ã‚¯ãƒˆ
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp
    glBindFramebuffer(GL_FRAMEBUFFER, _extractBrightFBO);
    glViewport(0, 0, _lowResWidth, _lowResHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    PostProcess();

<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
    // ƒXƒeƒbƒv4: UI‚ğÀƒEƒBƒ“ƒhƒEƒTƒCƒY‚Å‡¬ 
    glBindFramebuffer(GL_FRAMEBUFFER, _finalFBO);
    glViewport(0, 0, _screenWidth, _screenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
=======
    // ã‚¹ãƒ†ãƒƒãƒ—4: UIã‚’å®Ÿã‚¦ã‚£ãƒ³ãƒ‰ã‚¦ã‚µã‚¤ã‚ºã§åˆæˆ 
    glBindFramebuffer(GL_FRAMEBUFFER, _finalFBO);
    glViewport(0, 0, _windowWidth, _windowHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp
    glClear(GL_COLOR_BUFFER_BIT);

    DrawAndCombineUI(currentMainScene);

    // ƒXƒeƒbƒv3: ÅIo—ÍiƒEƒBƒ“ƒhƒE‚É•`‰æj
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
<<<<<<< HEAD:src/Renderer/Renderer2D.cpp
    glViewport(0, 0, _screenWidth, _screenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
=======
    glViewport(0, 0, _windowWidth, _windowHeight);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp
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

    // ãƒ¡ã‚¤ãƒ³ã‚·ãƒ¼ãƒ³ã‚’å–å¾—
    MainScene& currentMainScene = _game.GetSceneManager().GetCurrentMainScene();
    // ãƒ‡ãƒ•ã‚©ãƒ«ãƒˆãƒ•ãƒ¬ãƒ¼ãƒ ãƒãƒƒãƒ•ã‚¡ã«æç”»ã™ã‚‹ã“ã¨ã‚’æ˜ç¢ºã«ã™ã‚‹
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

// ===== ƒ‰ƒCƒgŠÖ˜A ===== //

/**
 * ƒ‰ƒCƒg‚ğ’Ç‰Á‚·‚é( ƒŒƒCƒ„[‚Ì”ÍˆÍ“à‚Ì‚İ—LŒø )
 */
void Renderer::SetAmbientLightFactor(int layer, AmbientLight factor) {
    if (layer >= 0 && layer < _numLayers) {
        // ŠÂ‹«ŒõŒW”‚ğ0.0`1.0‚Ì”ÍˆÍ‚É§ŒÀ
        _ambientLightFactors[layer] = factor;
    }
}

/**
 * ‘S‚Ä‚ÌƒŒƒCƒ„[‚ÌŠÂ‹«ŒõŒW”‚ğİ’è‚·‚é( ƒŒƒCƒ„[‚Ì”ÍˆÍ“à‚Ì‚İ—LŒø )
 */
void Renderer::SetAllAmbientLightFactor(AmbientLight factor) {
    for (int i = 0; i < _numLayers; i++) {
        SetAmbientLightFactor(i, factor);
    }
}

/**
 * w’èƒŒƒCƒ„[‚ÌŠÂ‹«ŒõŒW”‚ğæ“¾‚·‚é( ƒŒƒCƒ„[‚Ì”ÍˆÍ“à‚Ì‚İ—LŒø )
 */
AmbientLight Renderer::GetAmbientLightFactor(int layer) const {
    if (layer >= 0 && layer < _numLayers) {
        return _ambientLightFactors[layer];
    }
    return AmbientLight();
}

/**
 * ƒ‰ƒCƒg‚ğ’Ç‰Á‚·‚é
 */
void Renderer::AddLight(LightComponent* light) {
    _lights.emplace_back(light);
}

/**
 * ƒ‰ƒCƒg‚ğíœ‚·‚é
 */
void Renderer::RemoveLight(LightComponent* light) {
    auto iter = std::find(_lights.begin(), _lights.end(), light);
    if (iter != _lights.end()) {
        std::iter_swap(iter, _lights.end() - 1);
        _lights.pop_back();
    }
}


// ===== ƒŒƒCƒ„[ŠÖ˜A ===== //

/**
 * ƒpƒ‰ƒ‰ƒbƒNƒXŒW”‚Ìİ’è
 *
 * ŒW”‚Í0‚ğ’†‰›ƒŒƒCƒ„[‚Æ‚µA1‚ğÅ”w–ÊƒŒƒCƒ„[‚Æ‚·‚é
 * -‚Í‘O–Ê‚ÉA+‚Í”w–Ê‚ÉˆÚ“®‚·‚é
 */
void Renderer::SetParallaxFactor(int layer, float factor) {
    if (layer >= 0 && layer < _numLayers) {
        _parallaxFactors[layer] = factor;
    }
}

/**
 * ƒpƒ‰ƒ‰ƒbƒNƒXŒW”‚Ìæ“¾
 */
float Renderer::GetParallaxFactor(int layer) const {
    if (layer >= 0 && layer < _numLayers) {
        return _parallaxFactors[layer];
    }
    return 0.0f;
}

/**
 * ’†‰›ƒŒƒCƒ„[‚Ìİ’è
 *
 * ƒpƒ‰ƒ‰ƒbƒNƒXŒW”‚ÌŠî€‚Æ‚È‚éƒŒƒCƒ„[‚ğİ’è‚·‚é
 */
void Renderer::SetCentralLayer(int layer) {
    if (layer >= 0 && layer < _numLayers) {
        _centralLayer = layer;
    }
}


//==============================================================================
// ƒvƒ‰ƒCƒx[ƒgŠÖ”
//==============================================================================


// ===== ‰Šú‰»ˆ— ===== //

/**
 * ƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚Ì‰Šú‰»
 */
bool Renderer::InitializeFrameBuffer() {

    // ƒƒCƒ“ƒV[ƒ“—pƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚Ìì¬
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


    // ƒ‰ƒCƒg—pƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚Ìì¬
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

    // ƒtƒŒ[ƒ€‚Æƒ‰ƒCƒg‚Ì‡¬—pƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚Ìì¬
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

    // ƒuƒ‰ƒCƒgƒlƒX’Šo—pƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚Ìì¬
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

    // ƒuƒ‰[—p‚ÌƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚Ìì¬
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

    // ƒuƒ‹[ƒ€ƒGƒtƒFƒNƒg—pÅIƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚Ìì¬
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

    // ÅI•`Ê—pƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚Ìì¬
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


    // ƒfƒtƒHƒ‹ƒg‚ÌƒtƒŒ[ƒ€ƒoƒbƒtƒ@‚É–ß‚·
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}


// ===== •`Êˆ— ===== //

/**
 * ƒƒCƒ“ƒV[ƒ“‚Ì•`Êˆ—
 *
 * ’á‰ğ‘œ“xƒŒƒ“ƒ_ƒŠƒ“ƒOAƒJƒƒ‰ƒrƒ…[AƒAƒ“ƒrƒGƒ“ƒgƒ‰ƒCƒg‚Ìİ’è‚ğ‚µA
 * Œ»İ‚ÌƒƒCƒ“ƒV[ƒ“‚ğ•`Ê‚·‚é
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
 * ƒ‰ƒCƒg‚Ì•`Êˆ—
 *
 * ’á‰ğ‘œ“xƒŒƒ“ƒ_ƒŠƒ“ƒOAƒJƒƒ‰ƒrƒ…[‚ğİ’è‚µ
 * ƒ‰ƒCƒg‚ğ•`Ê‚·‚é
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
 * ƒŒƒCƒ„[‚ÌƒXƒvƒ‰ƒCƒg‚Æƒ‰ƒCƒg‚ğ‡¬‚·‚é
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
 * ƒ|ƒXƒgƒvƒƒZƒXˆ—
 *
 * ƒuƒ‹[ƒ€ƒGƒtƒFƒNƒg‚ğ“K—p‚·‚é
 */
void Renderer::PostProcess() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ƒuƒ‰ƒCƒgƒlƒX’Šo
    _extractBrightShader->SetActive();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _combineLightBuffer);
    _extractBrightShader->SetIntUniform("frameTexture", 0);

    _frameVerts->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // ƒuƒ‰[ˆ—
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

    // ƒuƒ‹[ƒ€ƒGƒtƒFƒNƒg
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
 * UI‚Ì•`ÊA‡¬ˆ—
 *
 * ƒƒCƒ“ƒV[ƒ“‚Ì•`Ê‚ğã‘‚«‚µAUI‚ğ•`Ê‚·‚é
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
 * UI‚Ì•`Êˆ—
 *
 * ÀƒEƒBƒ“ƒhƒEƒTƒCƒY‚É‡‚í‚¹‚ÄUI‚ğ•`Ê‚·‚é
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
 * ƒEƒBƒ“ƒhƒE‚ÌƒXƒƒbƒv
 *
 * ƒ_ƒuƒ‹ƒoƒbƒtƒ@ƒŠƒ“ƒO‚ğs‚¤
 */
void Renderer::SwapWindow() {
    SDL_GL_SwapWindow(_window);
}


// ===== ƒVƒF[ƒ_[ŠÖ˜A ===== //

/**
 * ƒVƒF[ƒ_[‚Ìƒ[ƒh
 *
 * ŠeíƒVƒF[ƒ_[‚Ìƒ†ƒjƒtƒH[ƒ€•Ï”‚Ìİ’è‚ğs‚¤
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

    // ƒVƒF[ƒ_[‚ÌŠî–{İ’è
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
 * ƒXƒvƒ‰ƒCƒg—p‚Ì’¸“_”z—ñ‚ğì¬
 *
 * ƒXƒvƒ‰ƒCƒg‚Ì•`Ê‚Ég—p‚·‚é’¸“_”z—ñ‚ğì¬‚·‚é
 */
void Renderer::CreateVerts() {
    // ƒXƒvƒ‰ƒCƒg—p’¸“_”z—ñ
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

    // ƒtƒŒ[ƒ€ƒoƒbƒtƒ@—p’¸“_”z—ñ
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


// ===== ƒVƒƒƒbƒgƒ_ƒEƒ“ˆ— ===== //

/**
 * ƒVƒƒƒbƒgƒ_ƒEƒ“ˆ—
 *
 * ƒƒ‚ƒŠ‚Ì‰ğ•úAOpenGL‚ÌƒRƒ“ƒeƒLƒXƒg‚Ì”jŠü‚ğs‚¤
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
>>>>>>> 544a8b5 (2Dãƒ¬ãƒ³ãƒ€ãƒ©ãŒå‡ºåŠ›ä¸å¯ã€å¯¾å‡¦ä¸­):src/Renderer/2D/Renderer2D.cpp

    SDL_GL_DestroyContext(_context);
    SDL_DestroyWindow(_window);
}