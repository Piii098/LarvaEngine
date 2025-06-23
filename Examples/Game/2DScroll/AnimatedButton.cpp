#include "Examples/2D/Scroll/AnimatedButton.h"
#include "LarvaEngine/Core/Utilities/Easing.h"

Example2DScroll::AnimatedButton::AnimatedButton(Scene& scene, const std::string& spriteName)
	: Button(scene, spriteName)
	, _startPos(Vector2::Zero)
	, _endPos(Vector2::Zero)
	, _duration(0.f)
	, _progress(0.f)
	, _isAnimating(false) {
}

Example2DScroll::AnimatedButton::~AnimatedButton() {
}

void Example2DScroll::AnimatedButton::UpdateObject(float deltaTime) {

	if (!_isAnimating) return;

	_elapsedTime += deltaTime;

	if (_elapsedTime < _delay) return;


	_progress += deltaTime / _duration;

	if (_progress >= 1.0f) {
		_progress = 1.0f;
		_isAnimating = false;
	}

	float easedProgress = Easing::EaseInOut(_progress);

	Vector2 newPos = Vector2::Lerp(_startPos, _endPos, easedProgress);

	Position(Vector3(newPos.x, newPos.y, 0));

}