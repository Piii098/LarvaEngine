#include <gl/glew.h>
#include "LarvaEngine/Renderer/3D/Renderer3D.h"
#include "LarvaEngine/Renderer/3D/VertexArray3D.h"
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/MainScene.h"
#include "larvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"

Renderer3D::Renderer3D(Game& game) 
    : Renderer(game){
	
}

Renderer3D::~Renderer3D() {

}

bool Renderer3D::Initialize(const std::string& windowName) {

	_windowHeight = _windowHeight == -1 ? 720 : _windowHeight;
	_windowWidth = _windowWidth == -1 ? 1280 : _windowWidth;

	if (_lowResHeight == -1 || _lowResWidth == -1) {
		_lowResHeight = _windowHeight;
		_lowResWidth = _windowWidth;
	}
	
    // OpenGL属性の設定
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	// ステンシルバッファを有効化するための設定を追加
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    _window = SDL_CreateWindow(windowName.c_str(), _windowWidth, _windowHeight, SDL_WINDOW_OPENGL);

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

	InitShadowMap();
	InitDOF();

	CreateVerts();

    return true;
}

void Renderer3D::Render() {
	

	glViewport(0, 0, 4096, 4096);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthFBO);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT );
	
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW); // 反時計回りを表面とする (Assimpのデフォルトと一致)

    // メインシーンを取得
    MainScene& currentMainScene = _game.GetSceneManager().GetCurrentMainScene();

	Camera* camera = currentMainScene.GetCamera();
	
	if (camera) {
		_view = camera->GetViewMatrix();
	}

	// シャドウマップのレンダリング

	Vector3 lightPos(-150.0f, 50.0f, 50.0f);

	Matrix4 lightProjection = Matrix4::CreateOrtho(1000, 1000, -10.0f, 1000.0f);
	Matrix4 lightView = Matrix4::CreateLookAt(lightPos, Vector3::Zero, Vector3::UnitZ);
	Matrix4 lightViewProj = lightView * lightProjection;

	_shadowShader->SetActive();
	_shadowShader->SetMatrixUniform("uLightViewProj", lightView * lightProjection);

	currentMainScene.RenderModel(*_shadowShader.get());
	Texture* shadowMap = new Texture(_game);
	shadowMap->TextureID(_depthBuffer);
	

	// 1.通常レンダリング
	glViewport(0, 0, _windowWidth, _windowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, _colorFBO);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_toonShader->SetActive();
	_toonShader->SetMatrixUniform("uViewProj", _view * _projection);

	Matrix4 invView = _view;
	invView.Invert();

	_toonShader->SetMatrixUniform("uInvMatrix", invView);
	_toonShader->SetMatrixUniform("uViewProj", _view * _projection);
	_toonShader->SetVector3Uniform("uLightDir", Vector3(-150.0f, 50.0f, 50.0f));

	_toonRampTexture = _game.GetTextureManager().Get("ToonRampTexture");

	_toonRampTexture->SetActive(1);
	_toonShader->SetIntUniform("uRampTexture", 1);

	_toonShader->SetMatrixUniform("uLightViewProj", lightViewProj);
	shadowMap->SetActive(2);
	_toonShader->SetIntUniform("uShadowMap", 2);
	glCullFace(GL_FRONT);
	currentMainScene.RenderModel(*_toonShader.get());

	// 2. ブラー処理のためのFBOへレンダリング

	bool horizontal = true, first_iteration = true;
	int amount = 5; 

	_blurShader->SetActive();

	for (unsigned int i = 0; i < amount; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[horizontal]);
		_blurShader->SetIntUniform("horizontal", horizontal);
		glBindTexture(
			GL_TEXTURE_2D, first_iteration ? _colorBuffer : _pingpongBuffers[!horizontal]
		);
		_frameVerts->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	glDisable(GL_CULL_FACE);


	// 3. 最終合成（被写界深度適用）
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	
	_frameVerts->SetActive();
	_dofShader->SetActive();

	_dofShader->SetMatrixUniform("uWorldTransform", Matrix4::Identity);
	
	_dofShader->SetIntUniform("sceneTexture", 0);
	_dofShader->SetIntUniform("blurTexture", 1);
	_dofShader->SetIntUniform("depthTexture", 2);

	// 被写界深度のパラメータ設定
	_dofShader->SetFloatUniform("focusDistance", 0.00005f); 
	_dofShader->SetFloatUniform("focusRange", 0.4f);    
	_dofShader->SetFloatUniform("maxBlur", 1.0f);       
	
	_dofShader->SetFloatUniform("near", 10.f);   
	_dofShader->SetFloatUniform("far", 10000.f);     

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _colorBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _pingpongBuffers[!horizontal]); 
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _colorDepthTexture);

	glDrawElements(GL_TRIANGLES, _frameVerts->NumIndices(), GL_UNSIGNED_INT, nullptr);

	_spriteVerts->SetActive();
	_spriteShader->SetActive();

	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_windowWidth, _windowHeight);

	_spriteShader->SetMatrixUniform("uViewProj", viewProj);
	_spriteShader->SetMatrixUniform("uViewCamera", Matrix4::Identity);

	_spriteShader->SetVector3Uniform("ambientLightColor", Vector3(1.0f, 1.0f, 1.0f));
	_spriteShader->SetFloatUniform("ambientLightIntensity", 1.0f);

	currentMainScene.RenderUIs(*_spriteShader.get());
	

	// Swap the window buffers
	SDL_GL_SwapWindow(_window);
}

bool Renderer3D::LoadShaders() {
	// シェーダーの読み込み
	_meshShader = std::make_unique<Shader>();
	if (!_meshShader->Load("Assets/Shaders/Phong.vert", "Assets/Shaders/Phong.frag")) {
		SDL_Log("Failed to load shader");
		return false;
	}

	_spriteShader = std::make_unique<Shader>();
	if (!_spriteShader->Load("Assets/Shaders/Sprite.vert", "Assets/Shaders/Sprite.frag")) {
		SDL_Log("Failed to load shader");
		return false;
	}

	_toonShader = std::make_unique<Shader>();
	if (!_toonShader->Load("Assets/Shaders/Toon.vert", "Assets/Shaders/Toon.frag")) {
		SDL_Log("Failed to load shader");
		return false;
	}

	_basicShader = std::make_unique<Shader>();
	if (!_basicShader->Load("Assets/Shaders/BasicMesh.vert", "Assets/Shaders/BasicMesh.frag")) {
		SDL_Log("Failed to load shader");
		return false;
	}

	_shadowShader = std::make_unique<Shader>();
	if (!_shadowShader->Load("Assets/Shaders/Shadow.vert", "Assets/Shaders/Shadow.frag")) {
		SDL_Log("Failed to load shader");
		return false;
	}

	_outlineShader = std::make_unique<Shader>();
	if (!_outlineShader->Load("Assets/Shaders/Outline.vert", "Assets/Shaders/Outline.frag")) {
		SDL_Log("Failed to load shader");
		return false;
	}

	_blurShader = std::make_unique<Shader>();
	if (!_blurShader->Load("Assets/Shaders/FrameBuffer.vert", "Assets/Shaders/Blur.frag")) {
		SDL_Log("Failed to load Blur shader");
		return false;
	}

	_frameShader = std::make_unique<Shader>();
	if (!_frameShader->Load("Assets/Shaders/FrameBuffer.vert", "Assets/Shaders/FrameBuffer.frag")) {
		SDL_Log("Failed to load shader");
		return false;
	}

	_dofShader = std::make_unique<Shader>();
	if (!_dofShader->Load("Assets/Shaders/FrameBuffer.vert", "Assets/Shaders/DOF.frag")) {
		SDL_Log("Failed to load shader");
		return false;
	}

	_frameShader->SetActive();
	_frameShader->SetMatrixUniform("uWorldTransform", Matrix4::Identity);

	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_windowWidth, _windowHeight);

	
	_spriteShader->SetActive();
	_spriteShader->SetMatrixUniform("uViewProj", viewProj);

	_meshShader->SetActive();

	_view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	_projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.f), _windowWidth, _windowHeight, 10.f, 10000.f);

	_meshShader->SetMatrixUniform("uViewProj", _view * _projection);

	_toonShader->SetActive();

	_toonShader->SetMatrixUniform("uViewProj", _view * _projection);
	_toonShader->SetIntUniform("uTexture", 0);
	_toonShader->SetIntUniform("uRampTexture", 1);
	
	_basicShader->SetActive();

	_basicShader->SetMatrixUniform("uViewProj", _view * _projection);
	
	_toonShader->SetVector3Uniform("uAmbientLight", Vector3(0.2f, 0.2f, 0.2f));
	_toonShader->SetVector3Uniform("uDirectionalLight.direction", Vector3(0.0f, -0.7f, -0.7f));
	_toonShader->SetVector3Uniform("uDirectionalLight.diffuseColor", Vector3(0.8f, 0.8f, 0.8f));

	_blurShader->SetActive();
	_blurShader->SetMatrixUniform("uWorldTransform", Matrix4::Identity);
	_blurShader->SetIntUniform("uTexture", 0);

	return true;
}

void Renderer3D::CreateVerts() {
	
	float spriteVertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
		0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f,
		-0.5f, -0.5f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f
	};

	unsigned int spriteIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	_spriteVerts = new VertexArray3D(spriteVertices, 4, spriteIndices, 6);

	float frameVertices[] = {
		-1.0f, 1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f,
		1.0f, 1.0f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f,
		1.0f, -1.0f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f,
		-1.0f, -1.0f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f
	};

	unsigned int frameIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	_frameVerts = new VertexArray3D(frameVertices, 4, frameIndices, 6);

}

void Renderer3D::SetLightUniforms(Shader& shader) {
	Matrix4 invView = _view;
	invView.Invert();
	shader.SetVector3Uniform("uCameraPos", invView.GetTranslation());
	shader.SetVector3Uniform("uAmbientLight", Vector3(0.2f, 0.2f, 0.2f));

	shader.SetVector3Uniform("uDirectionalLight.direction", Vector3(0.0f, -0.707f, -0.707f));
	shader.SetVector3Uniform("uDirectionalLight.diffuseColor", Vector3(0.78f, 0.88f, 1.0f));
	shader.SetVector3Uniform("uDirectionalLight.specularColor", Vector3(0.8f, 0.8f, 0.8f));
}

void Renderer3D::InitShadowMap() {
	// シャドウマップテクスチャの生成

	glGenFramebuffers(1, &_depthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthFBO);

	// デプステクスチャの作成
	glGenTextures(1, &_depthBuffer);
	glBindTexture(GL_TEXTURE_2D, _depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindFramebuffer(GL_FRAMEBUFFER, _depthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBuffer, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		SDL_Log("Final Bloom framebuffer is not complete!");
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer3D::InitDOF() {
	// シーンカラー用FBO
	glGenFramebuffers(1, &_colorFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _colorFBO);

	// カラーテクスチャの生成
	glGenTextures(1, &_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, _colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _windowWidth, _windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer, 0);

	// 深度バッファをテクスチャとして作成（RBOではなく）
	glGenTextures(1, &_colorDepthTexture);
	glBindTexture(GL_TEXTURE_2D, _colorDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, _windowWidth, _windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _colorDepthTexture, 0);


	// ブラー用のフレームバッファの作成
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
		}
	}

	// エラーチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		SDL_Log("DoF framebuffer setup is not complete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}