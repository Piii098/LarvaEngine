#pragma once
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

class GameObject;

struct AnimationFrame {
    Vector2 offset;  // テクスチャ内でのオフセット位置
	Vector2 scale;   // テクスチャのスケール
    int width;
    int height;// フレームのサイズ
    float duration;  // フレームの表示時間（秒）
};

struct AnimationClip {
    std::string name;
    std::vector<AnimationFrame> frames;
    bool isLooping;
    float totalDuration;
};

class AnimationComponent : public SpriteComponent {
public:
    AnimationComponent(GameObject& parentt, int bufferLayer = 0, int updateOrder = 200);
    ~AnimationComponent();

    void Update(float deltaTime) override;

   
    void AddAnimation(const std::string& name, const std::vector<AnimationFrame>& frames, bool isLooping = true);
    void AddAnimationFromSpriteSheet(const std::string& name, int frameWidth, int frameHeight,int startX, int startY, int frameCount,  float frameDuration, bool isLooping = true);

    void Play(const std::string& name, bool restart = false);
    void Stop();
    void Pause();
    void Resume();


    // 現在のアニメーション状態の取得
    bool IsPlaying() const { return _isPlaying; }
    bool IsPaused() const { return _isPaused; }
    const std::string& GetCurrentAnimationName() const { return _currentAnimName; }
    int GetCurrentFrameIndex() const { return _currentFrameIndex; }

private:
    void UpdateFrame();

    std::unordered_map<std::string, AnimationClip> _animations;
    std::string _currentAnimName;

    int _currentFrameIndex = 0;
    float _frameTimer = 0.0f;

    bool _isPlaying = false;
    bool _isPaused = false;

};