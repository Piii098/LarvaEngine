#include "LarvaEngine/Core/UIScene.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Core/Resources/Font.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

/**
 * 親のメインシーンに自信を追加する
 */
UIScene::UIScene(MainScene& parent)
	: Scene(parent.GetManager())
	, _parent(parent){
	
}

/**
 * 親のメインシーンから自身を削除する
 */
UIScene::~UIScene() {
	//_parent.RemoveUIScene(this);
}

//==============================================================================
// パブリック関数
//==============================================================================

/**
 * シーンの初期化
 * LoadData()を呼び出す
 * 特別な初期化処理が必要な場合はオーバーライドする
 */
void UIScene::Initialize() {
	LoadData();
}

/**
 * 入力処理
 * 基底シーンメソッドと独自の入力処理を呼び出す
 */
void UIScene::InputScene(const InputAction& input) {

	if (_state != STATE::ACTIVE) return; // 非アクティブ状態の場合は処理しない

	Scene::InputScene(input);
	InputUI(input);
}

/**
 * 更新処理
 * 基底シーンメソッドと独自の更新処理を呼び出す
 */
void UIScene::UpdateScene(const float deltaTime) {

	if (_state != STATE::ACTIVE) return; // 非アクティブ状態の場合は処理しない

	Scene::UpdateScene(deltaTime);
	UpdateUI(deltaTime);
}

/**
 * 描写処理
 * スプライト配列の描写処理を行う
 * Rendererで呼び出され実ウィンドウサイズでの描写処理を行う
 * カメラに依存せず、通常のシーンを上書きされる形で描写処理される
 */
void UIScene::Render(Shader& shader) {

	if (_state != STATE::ACTIVE) return; // 非アクティブ状態の場合は処理しない
	
	for (auto& spri : _sprites) {
		
		// スプライトがアクティブ状態かつ、親オブジェクトがアクティブ状態の場合
		if (spri->State() == Component::STATE::ACTIVE && spri->GetParent().State() == GameObject::STATE::ACTIVE) {	
			spri->Render(shader);
		}
	}
	
}
