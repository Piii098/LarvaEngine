#include "UI/PauseMenu.h"
#include "Scene/Scene.h"
#include "Utilities/Input.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Core/Game.h"
#include "AssetManagers/AssetManager.h"

#pragma region コンストラクタ:デストラクタ

PauseMenu::PauseMenu(Scene* scene)
	: UIScreen(scene) {
	GetScene()->SetState(Scene::STATE::PAUSE);
	SetTitle("ポーズ", Color::Black, 100);
	_titlePos = Vector2(-500.f, 200.f);
	//_test = GetScene()->GetManager()->GetGame()->GetTextureManager()->Get("RedBox");
}

PauseMenu::~PauseMenu() {
	GetScene()->SetState(Scene::STATE::GAME_PLAY);
}

#pragma endregion

#pragma region パブリック関数

void PauseMenu::ProcessInput(Input* input) {
	if (input->IsInputDown(InputMap::INPUT_BACK)) {
		Close();
	}
}

void PauseMenu::Update(float deltaTime) {
}

void PauseMenu::Render(Shader* shader) {
	DrawTexture(shader, _title, _titlePos);
	//DrawTexture(shader, _test, Vector2(10.f, 10.f));
}

#pragma endregion