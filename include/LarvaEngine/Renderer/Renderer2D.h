#pragma once
#include <vector>
#include <string>
#include <memory>
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/Utilities/Math.h" 

/// @brief アンビエントライト構造体
/// カラーと強度を持ち各レイヤーに設定する
struct AmbientLight {

    /// @brief 指定コンストラクタ
    /// @param color ライトカラー 
    /// @param intensity 強度
    AmbientLight(Vector3 color, float intensity) : color(color), intensity(intensity) {}
    
	/// @brief デフォルトコンストラクタ
	/// デフォルトのカラーと強度を設定する(影響がない)
	AmbientLight() : color(Vector3(1, 1, 1)), intensity(1.f) {}

	Vector3 color;    ///< カラー
	float intensity;  ///< 強度
};

// ===== 前方宣言 ===== //
class Game;
class VertexArray;
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
class Renderer {
public:

	// ===== コンストラクタ・デストラクタ =====//
    Renderer(Game& game);
    ~Renderer();


	// ===== 初期化処理 =====//

	/// @brief 初期化
	/// ウィンドウを作成し、OpenGLの初期化を行う
	/// @param screenWidth ウィンドウの幅
	/// @param screenHeight ウィンドウの高さ
	/// @param lowResWidth 低解像度レンダリングの幅
	/// @param lowResHeight 低解像度レンダリングの高さ
    bool Initialize(float screenWidth, float screenHeight, float lowResWidth, float lowResHeight, const std::string& windowName);
    

	/// @brief 描写処理
	/// メインシーンとUIシーンを描写する
	/// レイヤーごとの描写のち、ライトを描写する
	/// ポストプロセスを行い、UIを描写する
    void Render();

	
	// ===== レイヤー関連 ===== //

    /// @brief パララックス係数の設定 
	/// 係数は0を中央レイヤーとし、1を最背面レイヤーとする
	/// -は前面に、+は背面に移動する
    /// @param layer 指定レイヤー 
	/// @param factor パララテックス係数
    void SetParallaxFactor(int layer, float factor);

	/// @brief パララックス係数の取得
	/// @param layer 指定レイヤー
	/// @return パララックス係数
    float GetParallaxFactor(int layer) const;

	/// @brief 中央レイヤーの設定
	/// パララックス係数の基準となるレイヤーを設定する
	/// @param layer 中央レイヤー
    void SetCentralLayer(int layer);


	// ===== ライト関連 ===== //

	/// @brief アンビエントライト係数の設定
	/// 各レイヤーにアンビエントライトを設定する
	/// @param layer 指定レイヤー
	/// @param factor アンビエントライト係数
	void SetAmbientLightFactor(int layer, AmbientLight factor);

	/// @brief 全てのアンビエントライト係数の設定
	/// 各レイヤーにアンビエントライトを設定する
	/// @param factor アンビエントライト係数
	void SetAllAmbientLightFactor(AmbientLight factor);

	/// @brief アンビエントライト係数の取得
	/// @param layer 指定レイヤー
	/// @return アンビエントライト係数
	AmbientLight GetAmbientLightFactor(int layer) const;

	/// @brief ライトの追加
	/// ライトコンポーネントを追加する
	/// @param light ライトコンポーネント
	void AddLight(LightComponent* light);

	/// @brief ライトの削除
	/// ライトコンポーネントを削除する
	/// @param light ライトコンポーネント
	void RemoveLight(LightComponent* light);

	
	// ===== ゲッター ===== //

	SDL_Window* GetWindow() const { return _window; }

private:

	// ===== 初期化処理 ===== //
    
	/// @brief フレームバッファの初期化
	bool InitializeFrameBuffer();


	// ===== 描写処理 ===== //

	/// @brief メインシーンの描写
	/// @param view カメラのビュー行列
	/// @param region 描写するレイヤー
    void DrawMainScene(MainScene& mainScene, Matrix4 view, int region);
	
	/// @brief ライトの描写
	/// @param view カメラのビュー行列
	/// @param region 描写するレイヤー
	void DrawLight(Matrix4 view, int region);

	/// @brief ライトの結合
	/// @param region 描写するレイヤー
	void CombineLight(int region);

	/// @brief ポストプロセスの描写
	/// ブルームエフェクトを行う
	void PostProcess();

	/// @brief UIを合成
	/// メインシーンとUIシーンを合成する
	void DrawAndCombineUI(MainScene& mainScene);

	/// @brief UIの描写
	/// UIシーンを描写する
	void DrawUI(MainScene& mainScene);

	/// @brief ウィンドウのスワップ
	/// ダブルバッファリングを行う
    void SwapWindow();

	
	// ===== リソース関連 ===== //

	/// @brief シェーダーの読み込み
	/// @return 成功したらtrue
    bool LoadShaders();

	/// @brief 頂点配列の作成
    void CreateVerts();


	// ===== 終了処理 ===== //

	/// @brief 終了処理
    void Shutdown();


	// ===== フレームバッファ関連 ===== //

	// メインシーン関連
	std::vector<unsigned int> _layerFBOs;		///< レイヤーごとのフレームバッファ
	std::vector<unsigned int> _layerBuffers;	///< レイヤーごとのテクスチャ
	std::vector<unsigned int> _lightFBOs;		///< ライトごとのフレームバッファ
	std::vector<unsigned int> _lightBuffers;	///< ライトごとのテクスチャ
	unsigned int _combineLightFBO;				///< ライト結合用フレームバッファ
	unsigned int _combineLightBuffer;			///< ライト結合用バッファ
	
	// ブルームエフェクト関連
	unsigned int _extractBrightFBO;				///< ブライトネス抽出用フレームバッファ
	unsigned int _extractBrightBuffers[2];		///< ブライトネス抽出用バッファ
	unsigned int _pingpongFBO[2];				///< 	
	unsigned int _pingpongBuffers[2];			///<
	unsigned int _finalBloomFBO;				///< ブルームエフェクト用フレームバッファ
	unsigned int _finalBloomBuffer;				///< ブルームエフェクト用バッファ
	 
	// 最終描写関連
	unsigned int _finalFBO;						///< 最終描写用フレームバッファ
	unsigned int _finalBuffer;					///< 最終描写用バッファ
    int _numLayers;

    // パララックス関連
    std::vector<float> _parallaxFactors;		///< 各レイヤーの視差係数 (0.0=中央レイヤーと同じ、1.0=最背面)
    int _centralLayer;							///< 中央レイヤー (基準となるレイヤー)

	// アンビエントライト関連
	std::vector<AmbientLight> _ambientLightFactors;	///< 各レイヤーのアンビエントライト係数
	std::vector<LightComponent*> _lights;

    // 基本的なレンダリングリソース
	SDL_Window* _window;							///< ウィンドウ
	SDL_GLContext _context;							///< OpenGLコンテキスト
	std::unique_ptr<VertexArray> _frameVerts;			///< フレーム描写用頂点配列
	std::unique_ptr<VertexArray> _spriteVerts;			///< スプライト描写用頂点配列
	std::unique_ptr<Shader> _frameShader;				///< フレーム描写用シェーダー
	std::unique_ptr<Shader> _spriteShader;				///< スプライト描写用シェーダー
	std::unique_ptr<Shader> _lightShader;				///< ライト描写用シェーダー
	std::unique_ptr<Shader> _combineShader;				///< ライト結合用シェーダー
	std::unique_ptr<Shader> _extractBrightShader;		///< ブライトネス抽出用シェーダー
	std::unique_ptr<Shader> _blurShader;				///< ブラー用シェーダー
	std::unique_ptr<Shader> _finalBloomShader;			///< ブルームエフェクト用シェーダー

    // ウィンドウ設定
	float _screenWidth;							///< ウィンドウの幅
	float _screenHeight;						///< ウィンドウの高さ	
	float _lowResWidth;							///< 低解像度レンダリングの幅
	float _lowResHeight;						///< 低解像度レンダリングの高さ	

    // ゲームリソース
	Game& _game;								///< ゲームクラス

};

//余地: ライトをMainSceneに移動する(一貫性のため)
//余地: シェーダーをアセットマネージャーで管理する