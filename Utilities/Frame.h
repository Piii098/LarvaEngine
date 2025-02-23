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
private:

	Uint64 _tickCount;
	float _gameTime;
	float _deltaTime;
	float _currentFps;
	int _setFps;
};