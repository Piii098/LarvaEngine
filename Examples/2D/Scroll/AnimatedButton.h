#pragma once
#include "LarvaEngine/GameObjects/UI/Button.h"

namespace Example2DScroll {

	class AnimatedButton : public Button {
	public:
		AnimatedButton(Scene& scene, const std::string& spriteName);
		~AnimatedButton() override;

		void StartAnimation(const Vector2& startPos, const Vector2& endPos, float duration, float delay = 0.f) {
			_startPos = startPos;
			Position(Vector3(startPos.x, startPos.y,0));
			_endPos = endPos;
			_duration = duration;
			_progress = 0.0f;
			_elapsedTime = 0.0f;
			_delay = delay;
			_isAnimating = true;
		}

		void UpdateObject(float deltaTime) override;

	private:
		Vector2 _startPos;
		Vector2 _endPos;
		float _duration;
		float _progress;
		float _delay;
		float _elapsedTime;
		bool _isAnimating;
	};

}