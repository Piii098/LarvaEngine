#include "Renderer.h"
#include <gl/glew.h>
#include "Utilities/Shader.h"
#include "Polygon/VertexArray.h"
#include "Components/SpriteComponent.h"
#include "Game.h"
#include "GameObjects/Camera.h"

#pragma region コンストラクタ:デストラクタ

Renderer::Renderer(Game* game)
	: _game(game){

}

Renderer::~Renderer() {

}

#pragma endregion

#pragma region パブリック関数

bool Renderer::Initialize(float screenWidth, float screenHeight) {

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	/*シェーダー初期化*/

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // コアOpenGLプロファイルを設定

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // バージョンを設定
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // RGBA、8ビットのカラーバッファを設定
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // ダブルバッファを有効

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // ハードウェアアクセラレーションを設定

	_window = SDL_CreateWindow("Game", _screenWidth, _screenHeight, SDL_WINDOW_OPENGL); //ウィンドウ作成、OpenGL
	if (!_window) {
		SDL_Log("Failed to create a window : %s", SDL_GetError());
		return false;
	}

	_context = SDL_GL_CreateContext(_window);

	glewExperimental = GL_TRUE; // GLEW初期化
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW : %s", SDL_GetError());
		return false;
	}
	glGetError();

	if (!LoadShaders()) {
		SDL_Log("Failed to load shaders");
		return false;
	}

	CreateSpriteVerts();

	/*
	if (!InitializeFrameBuffer()) {
		SDL_Log("Failed to initialize FrameBuffer");
		return false;
	}
	*/

	return true;

}

void Renderer::Shutdown() {

}

void Renderer::Draw() {
	/*画面クリア*/

	glClearColor(0.86f, 0.86f, 0.86f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	/*シェーダー*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_spriteShader->SetActive();
	_spriteVerts->SetActive();

	Matrix4 view = _game->GetCamera()->GetViewMatrix();
	_spriteShader->SetMatrixUniform("uViewScreen", view);

	_spriteShader->SetVector3Uniform("ambientLightColor", Vector3(0.2, 0.2, 0.3));
	_spriteShader->SetFloatUniform("ambientLightIntensity", 1.f);

	_spriteShader->SetFloatUniform("brightness", 2.f);
	_spriteShader->SetVector2Uniform("pointLightPos", _lightPos);
	_spriteShader->SetVector3Uniform("pointLightColor", Vector3(1.0, 0.9, 0.7));
	_spriteShader->SetFloatUniform("pointLightIntensity", 1.f);
	_spriteShader->SetFloatUniform("pointLightRadius", 100.f);

	/*スプライトコンポーネント*/
	for (auto spri : _sprites) {
		spri->Draw(_spriteShader);
	}
	

	/*前面に表示*/

	SDL_GL_SwapWindow(_window);
}

void Renderer::AddSprite(SpriteComponent* sprite) {

	int layer = sprite->DrawLayer();
	auto iter = _sprites.begin();

	for (; iter != _sprites.end(); ++iter) { // 配列の最後まで

		if (layer < (*iter)->DrawLayer()) { // 階層数が既存のスプライトより小さいければ
			break; //抜ける
		}

	}

	_sprites.insert(iter, sprite); // 適切な位置に挿入する
}

void Renderer::RemoveSprite(SpriteComponent* sprite) {

	auto iter = std::find(_sprites.begin(), _sprites.end(), sprite);
	_sprites.erase(iter);
}

#pragma endregion

#pragma region プライベート関数

bool Renderer::InitializeFrameBuffer() {
	// フレームバッファオブジェクトの設定
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _screenWidth, _screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}

	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _screenWidth, _screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		SDL_Log("Framebuffer not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _screenWidth, _screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			SDL_Log("Pingpong framebuffer not complete!");
	}

	// シェーダーの読み込み
	bloomShader = new Shader();
	if (!bloomShader->Load("Shaders/Bloom.vert", "Shaders/Bloom.frag"))
	{
		SDL_Log("Failed to load bloom shaders");
		return false;
	}
}

void Renderer::ApplyBloom() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 既存のシーン描画コード

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	bloomShader->SetActive();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		bloomShader->SetIntUniform("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);
		// 四角形をレンダリング
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::CreateSpriteVerts() {

	float vertices[] = {
		-0.5f,  0.5f,  0.f, 0.f, 0.f, // top left
		 0.5f,  0.5f,  0.f, 1.f, 0.f, // top right
		 0.5f, -0.5f,  0.f, 1.f, 1.f, // bottom right
		-0.5f, -0.5f,  0.f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	_spriteVerts = new VertexArray(vertices, 4, indices, 6);

}

bool Renderer::LoadShaders() {
	_spriteShader = new Shader();
	if (!_spriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) {
		return false;
	}

	_spriteShader->SetActive();

	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_screenWidth, _screenHeight);

	_spriteShader->SetMatrixUniform("uViewProj", viewProj);

	return true;
}

#pragma endregion

