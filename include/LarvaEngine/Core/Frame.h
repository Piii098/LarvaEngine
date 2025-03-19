#pragma once
#include <SDL3/SDL.h>

class Frame {
public:
    Frame();
    ~Frame();

    void Update();

    /*ゲッターセッター*/
    Uint64 GameTime() const { return _gameTime; };
    float DeltaTime() const { return _deltaTime; };
    float Fps() const { return _currentFps; };

    // 固定タイムステップ関連
    void SetFixedDeltaTime(float fixedDeltaTime) { _fixedDeltaTime = fixedDeltaTime; }
    float GetFixedDeltaTime() const { return _fixedDeltaTime; }
    bool ShouldDoFixedUpdate() const { return _accumulator >= _fixedDeltaTime; }
    void ConsumeFixedDeltaTime() { _accumulator -= _fixedDeltaTime; }
    float GetAccumulator() const { return _accumulator; }

    // 補間係数を返す（0～1の値）
    float GetAlpha() const { return (_fixedDeltaTime > 0.f) ? _accumulator / _fixedDeltaTime : 0.f; }

private:
    Uint64 _tickCount;
    float _gameTime;
    float _deltaTime;
    float _currentFps;
    int _setFps;

    // 固定タイムステップ関連
    float _fixedDeltaTime; // 固定タイムステップ値
    float _accumulator; // 蓄積時間
};