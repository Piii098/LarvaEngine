﻿#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Resources/Font.h"
#include "LarvaEngine/Core/SceneManager.h"

// ===== 前方宣言 =====
class FrameSystem;
class SpriteComponent;
class Box2DComponent;
class Mesh;
class Model;
class GameObject;
class SceneManager;
class TileMap;
class Texture;
class Player;
class PhysWorld2D;
class PhysWorld3D;
class Renderer;
class Renderer2D;

template <typename T>
class AssetManager;
class TileMapObject;
class AudioSystem;
class InputSystem;
class InputAction;

///
///@brief ゲームのメインクラス
///
///ゲームの初期化、メインループ、シャットダウン処理を行う
///システムコンポーネントの管理、アセットマネージャーの管理を行う
/// 
class Game {
public:

	enum class RENDERER_MODE {
		MODE_2D,
		MODE_3D,
	};

    Game();
    ~Game();

	// ===== 初期設定 ===== //
	/// @brief 初期のシーンを設定する
	/// されなければMainSceneが初期のシーンとなる
	/// @tparam T 初期のシーンクラス(ManSceneを継承したクラス)
    template <typename T>
    void SetInitialScene() {
        _initialSceneSetup = [](SceneManager& manager) {
            manager.InitializeWithScene<T>();
            };
    }

    /// @brief ウィンドウサイズを設定する
    /// 低解像度レンダリングのサイズは整数倍のサイズにすること
	/// @param windowWidth ウィンドウの幅
    /// @param windowHeight ウィンドウの高さ
    /// @param lowResWidth 低解像度レンダリングの幅
    /// @param lowResHeight 低解像度レンダリングの高さ
    void SetWindowsSize(int windowWidth, int windowHeight, int lowResWidth = -1, int lowResHeight = -1);

	/// @brief ウィンドウ名を設定する
	/// @param name ウィンドウ名
    void SetWindowsName(const std::string& name);

	/// @brief 初期化処理を行う
	/// @return　初期化が成功したかどうか
	bool Initialize();         

	/// @brief メインループを実行する
	/// 初期化が成功したことを前提とする
	void RunLoop(); 	    		 

	/// @brief シャットダウン処理を行う
	void Shutdown();        

    // ===== ゲッターメソッド ===== //

	void RendererMode(RENDERER_MODE mode) { _rendererMode = mode; }

	// システムコンポーネント

    FrameSystem& GetFrameSystem() { return *_frameSystem.get(); }
	PhysWorld2D& GetPhysWorld2D() { return *_physWorld2D.get(); }
	PhysWorld3D& GetPhysWorld3D() { return *_physWorld3D.get(); }
    Renderer& GetRenderer()  { return *_renderer.get(); }
    AudioSystem& GetAudioSystem()  { return *_audioSystem.get(); }
    InputSystem& GetInputSystem()  { return *_inputSystem.get(); }
    InputAction& GetInputAction()  { return *_inputAction.get(); }
    SceneManager& GetSceneManager() { return *_sceneManager.get(); }
    const FrameSystem& GetFrameSystem() const { return *_frameSystem.get(); }
	const PhysWorld2D& GetPhysWorld2D() const { return *_physWorld2D.get(); }
	const PhysWorld3D& GetPhysWorld3D() const { return *_physWorld3D.get(); }
	const Renderer& GetRenderer() const { return *_renderer.get(); }
	const AudioSystem& GetAudioSystem() const { return *_audioSystem.get(); }
	const InputSystem& GetInputSystem() const { return *_inputSystem.get(); }
	const InputAction& GetInputAction() const { return *_inputAction.get(); }
	const SceneManager& GetSceneManager() const { return *_sceneManager.get(); }

    // アセットマネージャー

    AssetManager<Texture>& GetTextureManager() { return *_textureManager.get(); }
    AssetManager<TileMap>& GetTileMapManager() { return *_tileMapManager.get(); }
    AssetManager<Font>& GetFontManager() { return *_fontManager.get(); }
	AssetManager<Mesh>& GetMeshManager() { return *_meshManager.get(); }
	AssetManager<Model>& GetModelManager() { return *_modelManager.get(); }
	const AssetManager<Texture>& GetTextureManager() const { return *_textureManager.get(); }
	const AssetManager<TileMap>& GetTileMapManager() const { return *_tileMapManager.get(); }
	const AssetManager<Font>& GetFontManager() const { return *_fontManager.get(); }
	const AssetManager<Mesh>& GetMeshManager() const { return *_meshManager.get(); }

private:

    // ===== メインループ処理 ===== //

	/// @brief 入力処理を行う
    void ProcessInput();

	/// @brief ゲームの更新処理を行う
    void Update();

	/// @brief 物理更新処理を行う
    void FixedUpdate();

	/// @brief シーンの更新処理を行う
    void UpdateScene();

    void LateUpdate();

	/// @brief 出力処理を行う
    void ProcessOutput();

	/// @brief レンダリング処理を行う
    void Render();

    void SetAction();

    // ===== リソース管理 ===== //

	/// @brief シーンを読み込む
    void LoadScene();

	/// @brief リソースを解放する
    void UnloadData();

    // ===== ウィンドウ設定 ===== //

    int _windowWidth;
    int _windowHeight;
    int _lowResWidth;
    int _lowResHeight;
    std::string _windowName;

    // ===== システムコンポーネント ===== //

	std::unique_ptr<FrameSystem> _frameSystem;              ///< フレームシステム       
	std::unique_ptr<InputSystem> _inputSystem;              ///< 入力システム 
	std::unique_ptr<InputAction> _inputAction;              ///< 入力アクション
	std::unique_ptr<SceneManager> _sceneManager;            ///< シーンマネージャ
	std::unique_ptr<PhysWorld2D> _physWorld2D;			    ///< 物理世界   
	std::unique_ptr<PhysWorld3D> _physWorld3D;			    ///< 物理世界
	std::unique_ptr<AudioSystem> _audioSystem;				///< オーディオシステム
	std::unique_ptr<Renderer> _renderer;                    ///< レンダラー

    // ===== ゲーム状態 ===== //

	bool _isRunning;	                                    ///< ゲームが実行中かどうか
	std::function<void(SceneManager&)> _initialSceneSetup;  ///< 初期のシーンセットアップ
	RENDERER_MODE _rendererMode;                            ///< レンダラーのモード

	bool _useMultithreading;

	std::vector<std::function<void()>> _asyncTasks;

    // ===== アセットマネージャー ===== //

    std::unique_ptr<AssetManager<Texture>> _textureManager; ///<テクスチャ 
	std::unique_ptr<AssetManager<TileMap>> _tileMapManager; ///<タイルマップ
	std::unique_ptr<AssetManager<Font>> _fontManager;		///<フォント
	std::unique_ptr<AssetManager<Mesh>> _meshManager;		///<メッシュ
	std::unique_ptr<AssetManager<Model>> _modelManager;		///<モデル

};