#pragma once
#include <string>
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/Utilities/Math.h"


/// @brief UIシーンクラス
///
/// UIシーンの基底クラス
/// UIシーンの初期化、入力、更新処理を行う
/// ゲームオブジェクトを持ち
/// 実ウィンドウのサイズでかつ、通常のシーンを上書きされる形でカメラに依存されず描写処理される
class UIScene : public Scene {
public:

	// ===== コンストラクタ・デストラクタ ===== //

	UIScene(MainScene& parent);
	virtual ~UIScene();


	// ===== 初期設定 ===== //

	/// @brief シーンの初期化
	/// LoadData()を呼び出す
	/// 特別な初期化処理が必要な場合はオーバーライドする
	virtual void Initialize() override;

	void ReloadScene() override;

	// ===== ループ処理 ===== //

	/// @brief 入力処理
	/// 基底シーンメソッドと独自の入力処理を呼び出す
	/// @param input 入力情報
	void InputScene(const InputAction& input) override;
	
	/// @brief 独自の入力処理
	/// 仮想関数、独自の入力処理を行う
	/// @param input 入力情報
	virtual void InputUI(const InputAction& input) {};

	/// @brief 更新処理
	/// 基底シーンメソッドと独自の更新処理を呼び出す
	/// @param deltaTime フレーム間の時間
	void UpdateScene(float deltaTime) override;

	/// @brief 独自の更新処理
	/// 仮想関数、独自の更新処理を行う、
	/// @param deltaTime フレーム間の時間
	virtual void UpdateUI(float deltaTime) {};

	/// @brief 描写処理
	/// スプライト配列の描写処理を行う
	/// Rendererで呼び出され実ウィンドウサイズでの描写処理を行う
	/// カメラに依存せず、通常のシーンを上書きされる形で描写処理される
	/// @param shader シェーダー
	void Render(Shader& shader);

protected:

	/// @brief データの読み込み
	/// 仮想関数、ゲームオブジェクトの生成、リソースの読み込みを行う
	virtual void LoadData() override {};
	virtual void LoadObjects() override {};	

	MainScene& _parent;	///< メインシーン

};