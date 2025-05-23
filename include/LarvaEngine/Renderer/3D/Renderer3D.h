#pragma once
#include <vector>
#include <string>
#include <memory>
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/Utilities/Math.h" 
#include "LarvaEngine/Renderer/Renderer.h"

// ===== 前方宣言 ===== //
class VertexArray3D;
class Texture;
class Shader;
class Camera;
class MainScene;
class UIScene;
class LightComponent;


/// @brief レンダラークラス
/// ゲームの描画を管理する
/// レイヤーごとに描画すること描写する
/// パララックスを設定することで視差を表現する
/// アンビエントライトを設定することで各レイヤーに光を当てる
/// ライトコンポーネントを追加することでライトを追加する
/// メインシーンは低解像度レンダリング、UIシーンは実ウィンドウレンダリングでUIを重ねる形で描写する
class Renderer3D : public Renderer {
public:

	// ===== コンストラクタ・デストラクタ =====//
	Renderer3D(Game& game);
	~Renderer3D() override;


	// ===== 初期化処理 =====//

	/// @brief 初期化
	/// ウィンドウを作成し、OpenGLの初期化を行う
	/// @param screenWidth ウィンドウの幅
	/// @param screenHeight ウィンドウの高さ
	/// @param lowResWidth 低解像度レンダリングの幅
	/// @param lowResHeight 低解像度レンダリングの高さ
	bool Initialize(const std::string& windowName) override;

	/// @brief 描写処理
	/// メインシーンとUIシーンを描写する
	/// レイヤーごとの描写のち、ライトを描写する
	/// ポストプロセスを行い、UIを描写する
	void Render() override;

	/// @brief ライトの追加
	/// ライトコンポーネントを追加する
	/// @param light ライトコンポーネント
	void AddLight(LightComponent* light) override {}

	/// @brief ライトの削除
	/// ライトコンポーネントを削除する
	/// @param light ライトコンポーネント
	void RemoveLight(LightComponent* light) override {}


	// ===== ゲッター ===== //

	SDL_Window* GetWindow() const { return _window; }

	void SetLowResResolution(float width, float height) {
		_lowResWidth = width;
		_lowResHeight = height;
	}

private:

	// ===== リソース関連 ===== //

	/// @brief シェーダーの読み込み
	/// @return 成功したらtrue
	bool LoadShaders();

	/// @brief 頂点配列の作成
	void CreateVerts();

	void SetLightUniforms(Shader& shader);

	void InitShadowMap();
	void InitDOF();

	/// @brief 終了処理
	void Shutdown();

	std::unique_ptr<Shader> _meshShader; ///< シェーダー
	std::unique_ptr<Shader> _spriteShader; ///< ライトシェーダー
	std::unique_ptr<Shader> _toonShader; ///< トゥーンシェーダー
	std::unique_ptr<Shader> _depthShader; ///< ライトシェーダー
	std::unique_ptr<Shader> _frameShader;
	std::unique_ptr<Shader> _basicShader; ///< ポストプロセスシェーダー
	std::unique_ptr<Shader> _shadowShader; ///< ブルームシェーダー
	std::unique_ptr<Shader> _outlineShader; ///< ブルームシェーダー
	std::unique_ptr<Shader> _dofShader; ///< DOFシェーダー
	std::unique_ptr<Shader> _blurShader; ///< ブルームシェーダー

	unsigned int _depthFBO; ///< シャドウマップのフレームバッファオブジェクト
	unsigned int _depthBuffer; ///< シャドウマップのテクスチャ

	unsigned int _colorFBO;
	unsigned int _colorBuffer; ///< カラーバッファ
	unsigned int _dofDepthBuffer; ///< DOFの深度バッファ
	unsigned int _colorDepthTexture; ///< DOFのフレームバッファオブジェクト
	unsigned int _pingpongFBO[2];				///< 	
	unsigned int _pingpongBuffers[2];

	VertexArray3D* _spriteVerts; ///< メッシュの頂点配列
	VertexArray3D* _frameVerts; ///< スプライトの頂点配列

	Matrix4 _view;
	Matrix4 _projection;

	Texture* _toonRampTexture; ///< トゥーンシェーダー用のテクスチャ
	
	Vector3 _ambientLight; ///< アンビエントライトの色

};

//余地: ライトをMainSceneに移動する(一貫性のため)
//余地: シェーダーをアセットマネージャーで管理する