#include "Utilities/Frame.h"

#pragma region �R���X�g���N�^:�f�X�g���N�^

Frame::Frame() 
    : _tickCount(0)
    , _setFps(120)
    , _currentFps(0.f)
    {

}

Frame::~Frame() {

}

#pragma endregion

#pragma region ���C��
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
    // �ڕW�t���[�����Ԃ��v�Z (�~���b�P��)
    float frameTime = 1000.0f / _setFps; // 1�b = 1000�~���b

    // ���݂̎��Ԃ��擾
    Uint64 currentTime = SDL_GetTicks();
    Uint64 currentFramedTime = currentTime - _tickCount; // �o�ߎ��� (ms)

    // �t���[�����Ԃ��ڕW���Z���ꍇ�͑ҋ@
    if (currentFramedTime < frameTime) {
        SDL_Delay(frameTime - currentFramedTime);
        currentTime = SDL_GetTicks(); // �x����Ɍ��ݎ��Ԃ��X�V
        currentFramedTime = currentTime - _tickCount;
    }

    // `_deltaTime` ��b�P�ʂŌv�Z (��� float �ɃL���X�g)
    _deltaTime = static_cast<float>(currentFramedTime) / 1000.0f;

    // �t���[���J�n���Ԃ��X�V
    _tickCount = currentTime;
    _gameTime = _tickCount / 1000.f;

    // FPS ���v�Z
    _currentFps = (_deltaTime > 0) ? (1.0f / _deltaTime) : 0.0f;
}

#pragma endregion