#pragma once
#include <SDL3/SDL.h>

/// @brief フレームシステムクラス
///
/// ゲームのフレーム処理を管理するクラス
/// ゲーム時間、デルタタイム、FPSを計測する
/// 固定タイムステップ処理を行う
class FrameSystem {
public:

	// ===== コンストラクタ・デストラクタ ===== //
    
    FrameSystem();
    ~FrameSystem();

	// ===== 更新処理 ===== //

	/// @brief フレームシステムの更新処理
	/// ゲーム時間、デルタタイム、FPSを計測する
	/// ゲームクラス内で毎フレーム呼び出す
    void Update();


	// ===== 固定タイムステップ処理 ===== //

	/// @brief 固定タイムステップ処理
	/// 固定タイムステップ処理を行うかどうかを返す
	/// @return true: 固定タイムステップ処理を行う
    bool GetShouldDoFixedUpdate() const { return _accumulator >= _fixedDeltaTime; }
    
	/// @brief 固定タイムステップ処理
	/// 蓄積時間から固定タイムステップを引く
	/// @return true: 固定タイムステップ処理を行う
    void GetConsumeFixedDeltaTime() { _accumulator -= _fixedDeltaTime; }
    
	/// @brief アルファ値を取得
	/// フレーム間の時間を固定タイムステップで割る
	/// @return アルファ値
    float GetAlpha() const { return (_fixedDeltaTime > 0.f) ? _accumulator / _fixedDeltaTime : 0.f; }


	// ===== ゲッターセッター ===== //
    
    Uint64 GameTime() const { return _gameTime; };
    float DeltaTime() const { return _deltaTime; };
    float Fps() const { return _currentFps; };

    // 固定タイムステップ関連
    void FixedDeltaTime(float fixedDeltaTime) { _fixedDeltaTime = fixedDeltaTime; }
    float FixedDeltaTime() const { return _fixedDeltaTime; }
    float Accumulator() const { return _accumulator; }

private:
    
	Uint64 _tickCount;          ///< 現在の時間
	float _gameTime;            ///< ゲーム時間  
	float _deltaTime;           ///< フレーム間の時間
	float _currentFps;          ///< 現在のFPS
	int _setFps;                ///< 目標FPS

    // 固定タイムステップ関連
    float _fixedDeltaTime;      ///< 固定タイムステップ値
    float _accumulator;         ///< 蓄積時間
};