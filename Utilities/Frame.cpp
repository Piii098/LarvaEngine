#include "Utilities/Frame.h"

#pragma region コンストラクタ:デストラクタ

Frame::Frame() 
    : _tickCount(0)
    , _setFps(120)
    , _currentFps(0.f)
    {

}

Frame::~Frame() {

}

#pragma endregion

#pragma region メイン
/*
void Frame::Update() {

    while (!TickPassed(SDL_GetTicks(), _tickCount + 16));

    float deltaTime = (SDL_GetTicks() - _tickCount) / 1000.f;

    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    _tickCount = SDL_GetTicks();

}
*/

void Frame::Update() {
    // 目標フレーム時間を計算 (ミリ秒単位)
    float frameTime = 1000.0f / _setFps; // 1秒 = 1000ミリ秒

    // 現在の時間を取得
    Uint64 currentTime = SDL_GetTicks();
    Uint64 currentFramedTime = currentTime - _tickCount; // 経過時間 (ms)

    // フレーム時間が目標より短い場合は待機
    if (currentFramedTime < frameTime) {
        SDL_Delay(frameTime - currentFramedTime);
        currentTime = SDL_GetTicks(); // 遅延後に現在時間を更新
        currentFramedTime = currentTime - _tickCount;
    }

    // `_deltaTime` を秒単位で計算 (先に float にキャスト)
    _deltaTime = static_cast<float>(currentFramedTime) / 1000.0f;

    // フレーム開始時間を更新
    _tickCount = currentTime;
    _gameTime = _tickCount / 1000.f;

    // FPS を計算
    _currentFps = (_deltaTime > 0) ? (1.0f / _deltaTime) : 0.0f;
}

#pragma endregion