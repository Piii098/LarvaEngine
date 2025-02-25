#include "Renderer.h"
#include <gl/glew.h>
#include "Utilities/Shader.h"
#include "Polygon/VertexArray.h"
#include "Components/SpriteComponent.h"
#include "Components/TextureComponent.h"
#include "Components/BGComponent.h"
#include "Game.h"
#include "GameObjects/Camera.h"

#pragma region コンストラクタ:デストラクタ

Renderer::Renderer(Game* game)
	: _game(game)
	, _isBloom(true)
	, _lowResWidth(0.f)
	, _lowResHeight(0.f){

}

Renderer::~Renderer() {

}

#pragma endregion

#pragma region パブリック関数

bool Renderer::Initialize(float screenWidth, float screenHeight, float lowResWidth, float lowResHeight) {

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_lowResWidth = lowResWidth;
	_lowResHeight = lowResHeight;

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
	delete _screenVerts;
	_screenVerts = nullptr;
	_frameBufferShader->Unload();
	_spriteShader->Unload();
	_backgroundShader->Unload();

	SDL_GL_DestroyContext(_context);
	SDL_DestroyWindow(_window);
}

void Renderer::UnloadData() {

}

void Renderer::Draw() {
	
	/*画面クリア*/

	glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
	glViewport(0, 0, _lowResWidth, _lowResHeight);
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/*シェーダー*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawBackground();
	DrawSprite();
	
	_upscaleShader->SetActive();

	glBindFramebuffer(GL_FRAMEBUFFER, _upscaleFBO);
	glViewport(0, 0, _screenWidth, _screenHeight);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _colorBuffer[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _colorBuffer[1]);

	_screenVerts->SetActive();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	
	bool horizontal = true;
	bool firstIteration = true;
	int amount = 20;

	_blurShader->SetActive();


	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[horizontal]);
		_blurShader->SetIntUniform("horizontal", horizontal);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture( GL_TEXTURE_2D, firstIteration ? _upscaleBuffer[1] : _pingpongBuffer[!horizontal]);
		// スクリーン全体を覆う四角形を描画
		_screenVerts->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		horizontal = !horizontal;
		if (firstIteration) firstIteration = false;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // デフォルトのフレームバッファに戻す
	glViewport(0, 0, _screenWidth, _screenHeight);
	glClear(GL_COLOR_BUFFER_BIT); // フレームバッファの内容を画面に描画
	
	_bloomFinalShader->SetActive();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _upscaleBuffer[0] );
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _pingpongBuffer[!horizontal]);
	_bloomFinalShader->SetIntUniform("uIsBloom", true);
	_bloomFinalShader->SetFloatUniform("exposure", 1.0f);
	float zoom = _game->GetCamera()->Zoom();
	_bloomFinalShader->SetFloatUniform("zoom", zoom);

	// スクリーン全体を覆う四角形を描画
	_screenVerts->SetActive();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	
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

void Renderer::AddBackground(BGComponent* background) {

	int layer = background->DrawLayer();
	auto iter = _backgrounds.begin();

	for (; iter != _backgrounds.end(); ++iter) { // 配列の最後まで

		if (layer < (*iter)->DrawLayer()) { // 階層数が既存のスプライトより小さいければ
			break; //抜ける
		}

	}

	_backgrounds.insert(iter, background); // 適切な位置に挿入する
}

void Renderer::RemoveBackground(BGComponent* background) {

	auto iter = std::find(_backgrounds.begin(), _backgrounds.end(), background);
	_backgrounds.erase(iter);
}

#pragma endregion

#pragma region プライベート関数

bool Renderer::InitializeFrameBuffer() {

	// フレームバッファオブジェクトの作成
	glGenFramebuffers(1, &_upscaleFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _upscaleFBO);

	// カラーテクスチャの作成
	glGenTextures(2, _upscaleBuffer);

	for (int i = 0; i < 2; i++) {

		glBindTexture(GL_TEXTURE_2D, _upscaleBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _screenWidth, _screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// フレームバッファにテクスチャをアタッチ
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _upscaleBuffer[i], 0);

	}

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	// フレームバッファの状態チェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		SDL_Log("Framebuffer is not complete!");
		return false;
	}


	// フレームバッファオブジェクトの作成
	glGenFramebuffers(1, &_hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);

	// カラーテクスチャの作成
	glGenTextures(2, _colorBuffer);

	for (int i = 0; i < 2; i++) {

		glBindTexture(GL_TEXTURE_2D, _colorBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _lowResWidth, _lowResHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// フレームバッファにテクスチャをアタッチ
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorBuffer[i], 0);

	}

	glDrawBuffers(2, attachments);

	// フレームバッファの状態チェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		SDL_Log("Framebuffer is not complete!");
		return false;
	}

	glGenFramebuffers(2, _pingpongFBO);
	// カラーテクスチャの作成
	glGenTextures(2, _pingpongBuffer);

	for (int i = 0; i < 2; i++) {

		glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, _pingpongBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _screenWidth, _screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// フレームバッファにテクスチャをアタッチ
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pingpongBuffer[i], 0);

	}
	

	// デフォルトのフレームバッファに戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void Renderer::UpScale() {

}

void Renderer::ApplyBloom(bool& horizontal) {
	bool firstIteration = true;
	int amount = 5;

	_blurShader->SetActive();

	for (int i = 0; i < amount; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[horizontal]);
		glViewport(0, 0, _screenWidth, _screenHeight);
		_blurShader->SetIntUniform("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, firstIteration ? _colorBuffer[1] : _pingpongBuffer[!horizontal]);
		_screenVerts->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		horizontal = !horizontal;
		if (firstIteration) firstIteration = false;
	}
}

void Renderer::DrawSprite() {
	_spriteShader->SetActive();
	_spriteVerts->SetActive();

	Matrix4 view = _game->GetCamera()->GetViewMatrix();
	_spriteShader->SetMatrixUniform("uViewScreen", view);

	_spriteShader->SetVector3Uniform("ambientLightColor", Vector3(1.f, 1.f, 1.f));
	_spriteShader->SetFloatUniform("ambientLightIntensity", 0.1f);

	_spriteShader->SetVector2Uniform("pointLightPos", _lightPos);
	_spriteShader->SetVector3Uniform("pointLightColor", Vector3(1.0, 0.9, 0.8));
	_spriteShader->SetFloatUniform("pointLightIntensity", 1.f);
	_spriteShader->SetFloatUniform("pointLightRadius", 100.f);

	/*スプライトコンポーネント*/
	for (auto spri : _sprites) {
		spri->Draw(_spriteShader);
	}
}

void Renderer::DrawBackground() {
	_backgroundShader->SetActive();
	_screenVerts->SetActive();

	_backgroundShader->SetVector3Uniform("ambientLightColor", Vector3(1.0f, 1.0f, 1.0f)); // 白 (無調整)
	_backgroundShader->SetFloatUniform("ambientLightIntensity", 0.1f);

	for (auto bg : _backgrounds) {
		bg->Draw(_backgroundShader);
	}
}

void Renderer::CreateSpriteVerts() {

	float vertices[] = {
		-0.5f,  0.5f, 0.f,  0.f, 0.f, // top left
		 0.5f,  0.5f, 0.f,  1.f, 0.f, // top right
		 0.5f, -0.5f, 0.f,  1.f, 1.f, // bottom right
		-0.5f, -0.5f, 0.f,  0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	_spriteVerts = new VertexArray(vertices, 4, indices, 6);

	float quadVertices[] = {
		// positions(x,y,z)    // texture coords(u,v)
		-1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // 左上
		 1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // 右上
		 1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // 右下
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f   // 左下
	};

	unsigned int quadIndices[] = {
		0, 1, 2,  // 最初の三角形
		0, 2, 3   // 二番目の三角形
	};

	_screenVerts = new VertexArray(quadVertices, 4, quadIndices, 6);
}


bool Renderer::LoadShaders() {
	_spriteShader = new Shader();
	if (!_spriteShader->Load("Shaders/Sprite.vert", "Shaders/LightSprite.frag")) {
		SDL_Log("Failed to laod SpriteShader");
		return false;
	}

	_backgroundShader = new Shader();
	if (!_backgroundShader->Load("Shaders/Background.vert", "Shaders/Background.frag")) {
		SDL_Log("Failed to laod BackgroundShader");
		return false;
	}

	_blurShader = new Shader();
	if (!_blurShader->Load("Shaders/FrameBuffer.vert", "Shaders/Blur.frag")) {
		SDL_Log("Failed to laod BlurShader");
		return false;
	}

	_upscaleShader = new Shader();
	if (!_upscaleShader->Load("Shaders/FrameBuffer.vert", "Shaders/Upscale.frag")) {
		SDL_Log("Failed to laod BlurShader");
		return false;
	}
	
	_bloomFinalShader = new Shader();
	if (!_bloomFinalShader->Load("Shaders/FrameBuffer.vert", "Shaders/BloomFinal.frag")) {
		SDL_Log("Failed to laod BloomFinalShader");
		return false;
	}

	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(_lowResWidth, _lowResHeight);

	_spriteShader->SetActive();
	_spriteShader->SetMatrixUniform("uViewProj", viewProj);

	_blurShader->SetActive();
	_blurShader->SetIntUniform("image", 0);

	_upscaleShader->SetActive();
	_upscaleShader->SetIntUniform("normalBuffer", 0);
	_upscaleShader->SetIntUniform("brightBuffer", 1);

	_bloomFinalShader->SetActive();
	_bloomFinalShader->SetIntUniform("scene", 0);
	_bloomFinalShader->SetIntUniform("bloomBlur", 1);

	return true;
}

#pragma endregion

