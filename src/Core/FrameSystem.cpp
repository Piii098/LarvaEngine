#include "LarvaEngine/Core/FrameSystem.h"
#include "LarvaEngine/Core/Utilities/Math.h"

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

FrameSystem::FrameSystem()
    : _gameTime(0.f)
	, _deltaTime(0.f)
	, _fps(0.f)
	, _fpsTimer(0.f)
	, _frameCount(0)
    , _fixedDeltaTime(1.f / 60.f)
	, _accumulator(0.f)
	, _lastTick(SDL_GetTicksNS()){
}

FrameSystem::~FrameSystem() {
}


//==============================================================================
// パブリック関数
//==============================================================================

/*
 * フレームシステムの更新処理主観混同
 * 
 * 設定したFPSに合わせてフレーム時間を計算し
 * 目標フレーム時間に達するまで待機する
 */
void FrameSystem::Update() {
    // 現在時間の取得
    Uint64 currentTicks = SDL_GetTicksNS();
    _deltaTime = static_cast<float>(currentTicks - _lastTick) / 1000000000.f;
    _lastTick = currentTicks;
	//SDL_Log("DeltaTime: %f", _deltaTime);
    // 極端なデルタタイムを制限（デバッグ停止等の対策）
    if (_deltaTime > 0.25f) _deltaTime = 0.25f;

    // アキュムレータに時間を追加
    _accumulator += _deltaTime;

    // フレーム数をカウント
    _frameCount++;

    // FPS計測用タイマーを更新
    _fpsTimer += _deltaTime;
    if (_fpsTimer >= 1.0f) {
        // FPSを更新
        _fps = static_cast<float>(_frameCount) / _fpsTimer;

        // カウントとタイマーをリセット
        _frameCount = 0;
        _fpsTimer = 0.f;

        // デバッグ用ログ（任意）
        SDL_Log("FPS: %d", _fps);
    }


	//SDL_Log("FPS: %f", _fps);
    // ゲーム時間の更新
    _gameTime += _deltaTime;
}

bool FrameSystem::ShouldRunFixedTimeStep() {
	// 固定タイムステップの処理
	if (_accumulator >= _fixedDeltaTime) {
		_accumulator -= _fixedDeltaTime;
		//SDL_Log("_accumulator: %f", _accumulator);
		return true;
	}
	return false;
}   