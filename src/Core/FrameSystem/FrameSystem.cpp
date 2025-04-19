#include "LarvaEngine/Core/FrameSystem.h"
#include "LarvaEngine/Core/Utilities/Math.h"

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

FrameSystem::FrameSystem()
    : _gameTime(0.f)
	, _deltaTime(0.f)
	, _lastTick(SDL_GetTicks()){
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
	Uint64 currentTick = SDL_GetTicks();
	Uint64 previousTicks = currentTick - _lastTick;
	_lastTick = currentTick;

	_deltaTime = static_cast<float>(previousTicks) / 1000.f; // 秒単位に変換

	_gameTime += _deltaTime;

	// FPS計算
	if (_deltaTime > 0.f) {
		_fps = 1.f / _deltaTime;
	}
	else {
		_fps = 0.f;
	}

    // 目標フレーム時間(ミリ秒)の計算（_targetFPSは設定済みのFPS）
    Uint32 targetFrameTicks = 1000 / _targetFPS;
    if (previousTicks < targetFrameTicks) {
        SDL_Delay(targetFrameTicks - static_cast<Uint32>(previousTicks));
        currentTick = SDL_GetTicks();
        previousTicks = currentTick - _lastTick;
    }
    _lastTick = currentTick;

    _deltaTime = static_cast<float>(previousTicks) / 1000.f; // 秒単位に変換
    _gameTime += _deltaTime;

    // FPS計算
    if (_deltaTime > 0.f) {
        _fps = 1.f / _deltaTime;
    }
    else {
        _fps = 0.f;
    }
}