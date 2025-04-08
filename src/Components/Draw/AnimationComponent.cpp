#include "LarvaEngine/Components/Draw/AnimationComponent.h"
#include <SDL3/SDL.h>

AnimationComponent::AnimationComponent(GameObject& parent, int bufferLayer, int updateOrder)
    : SpriteComponent(parent, bufferLayer, updateOrder) {
}

AnimationComponent::~AnimationComponent() {
}

void AnimationComponent::Update(float deltaTime) {
    // アニメーションが再生中で、一時停止中でない場合
    if (_isPlaying && !_isPaused) {
        _frameTimer += deltaTime;

        // 現在のフレームの表示時間を超えた場合
        if (_frameTimer >= _animations[_currentAnimName].frames[_currentFrameIndex].duration) {
            _frameTimer = 0.0f;
            _currentFrameIndex++;

            // 最後のフレームを超えた場合
            if (_currentFrameIndex >= _animations[_currentAnimName].frames.size()) {
                // ループする場合
                if (_animations[_currentAnimName].isLooping) {
                    _currentFrameIndex = 0;
                }
                else {
                    _isPlaying = false;
                    _currentFrameIndex = _animations[_currentAnimName].frames.size() - 1;
                }
            }

            UpdateFrame();

        }
    }

    // 基底クラスのUpdateを呼び出して描画を行う
    //TextureComponent::Update(deltaTime);
}

void AnimationComponent::AddAnimation(const std::string& name, const std::vector<AnimationFrame>& frames, bool isLooping) {
    AnimationClip clip;
    clip.name = name;
    clip.frames = frames;
    clip.isLooping = isLooping;

    clip.totalDuration = 0.0f;
    for (const auto& frame : frames) {
        clip.totalDuration += frame.duration;
    }

    _animations.emplace(name, clip);
}

void AnimationComponent::AddAnimationFromSpriteSheet(const std::string& name, int frameWidth, int frameHeight,
    int startX, int startY, int frameCount, float frameDuration, bool isLooping) {
    std::vector<AnimationFrame> frames;
	int framePerCol = _texHeight / frameHeight;
	int framesPerRow = _texWidth / frameWidth;

    for (int col = 0; col < framePerCol; col++) {

        for (int row = 0; row < framesPerRow; row++) {
			AnimationFrame frame;
			frame.offset.x = static_cast<float>(row) / framesPerRow;
			frame.offset.y = static_cast<float>(col) / framePerCol;
			frame.width = frameWidth;
			frame.height = frameHeight;
			frame.scale.x = 1.f / framesPerRow;
			frame.scale.y = 1.f / framePerCol;
			frame.duration = frameDuration;
			frames.emplace_back(frame);
                
			if (frames.size() == frameCount) break;
        }
		if (frames.size() == frameCount) break;
    }


    AddAnimation(name, frames, isLooping);
}

void AnimationComponent::Play(const std::string& name, bool restart) {
    // アニメーションが存在しない場合は何もしない
    if (_animations.find(name) == _animations.end()) {
        return;
    }

    // アニメーションが再生中で、再生するアニメーションが同じで、再生しなおす場合
    if (_isPlaying && _currentAnimName == name && !restart) {
        return;
    }
    else {
        _currentAnimName = name;
        _currentFrameIndex = 0;
        _frameTimer = 0.0f;
        _isPlaying = true;
        _isPaused = false;

        // フレーム情報を更新
        UpdateFrame();
    }
}

void AnimationComponent::Stop() {
    _isPlaying = false;
    _currentFrameIndex = 0;
    _frameTimer = 0.0f;
}

void AnimationComponent::Pause() {
    _isPaused = true;
}

void AnimationComponent::Resume() {
    _isPaused = false;
}

void AnimationComponent::UpdateFrame() {
    if (_animations.empty() || _currentAnimName.empty() ||
        _currentFrameIndex >= _animations[_currentAnimName].frames.size()) {
        return;
    }

    const auto& currentFrame = _animations[_currentAnimName].frames[_currentFrameIndex];

    if (_texture) {
       
		_texOffset = currentFrame.offset;
		_texScale = currentFrame.scale;

    }
}