#include "UI/UIScreen.h"
#include "Scene/Scene.h"
#include "Renderer/Shader.h"
#include "AssetManagers/AssetData/Texture.h"
#include "Utilities/Font.h"
#include "Scene/SceneManager.h"
#include "Core/Game.h"
#include "AssetManagers/AssetManager.h"

#pragma region コンストラクタ:デストラクタ

UIScreen::UIScreen(Scene* scene)
	: _scene(scene)
	, _title(nullptr)
	, _titlePos(Vector2(0.f, 0.f))
	, _state(STATE::ACTIVE){
	_scene->PushUI(this);
	_font = GetScene()->GetManager()->GetGame()->GetFontManager()->Get("DelaSuko");

}

UIScreen::~UIScreen() {
	if (_title) {
		_title->Unload();
		delete _title;
		_title = nullptr;
	}

}

#pragma endregion

#pragma region パブリック関数

void UIScreen::ProcessInput(Input* input) {

}

void UIScreen::HandleKeyPress(int Key) {

}

void UIScreen::Update(float deltaTime) {
	
}

void UIScreen::Render(Shader* shader) {
	if (_title) {
		DrawTexture(shader, _title, _titlePos);
	}
}

void UIScreen::SetTitle(const std::string& text, const Vector3& color, int pointSize) {
	if (_title) {
		_title->Unload();
		delete _title;
		_title = nullptr;
	}
	_title = _font->RenderText(text, color, pointSize);
}

#pragma endregion

#pragma region プライベート関数

void UIScreen::DrawTexture(Shader* shader, Texture* texture, const Vector2& offset, float scale) {

	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(texture->Width()), static_cast<float>(texture->Height()), 1.0f);
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));
	Matrix4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	shader->SetVector2Uniform("uTexOffset", Vector2(1.f, 1.f));
	shader->SetVector2Uniform("uTexScale", Vector2(1.f, 1.f));
	shader->SetVector3Uniform("selfLightColor", Vector3(1.f, 1.f, 1.f));
	shader->SetFloatUniform("selfLightIntensity", 1.f);

	texture->SetActive();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}

#pragma endregion