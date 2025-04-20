#pragma once
#include <SDL3/SDL.h>

/// @brief フレームシステムクラス
///
/// ゲームのフレーム処理を管理するクラス
/// ゲーム時間、デルタタイム、FPSを計測する
/// 固定タイムステップ処理を行う
class FrameSystem {
public:

	FrameSystem();
	~FrameSystem();

	/// @brief 更新処理
	/// 
	/// デルタタイムの計算
	void Update();

	/// @brief 固定タイムステップの処理
	///
	/// @return true: 固定タイムステップの処理を行う
	/// @return false: 固定タイムステップの処理を行わない
	bool ShouldRunFixedTimeStep();


	// ----- アクセサ ----- //

	float DeltaTime() const { return _deltaTime; }
	float GameTime() const { return _gameTime; }
	float FPS() const { return _fps; }
	float FixedDeltaTime() const { return _fixedDeltaTime; }

private:
	float _deltaTime;	///< 前フレームからの経過時間
	float _gameTime;    ///< ゲーム時間
	Uint64 _lastTick;	///< 前回のフレーム時間
	float _fps;		    ///< フレームレート
	float _accumulator;	///< フレーム時間の蓄積
	float _fixedDeltaTime;	///< 固定フレーム時間
};