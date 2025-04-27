#pragma once
#include "LarvaEngine/Core/Utilities/Math.h"

namespace Easing {

	inline float EaseInOut(float t) {
		return t < 0.5f ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
	}

	inline float EaseIn(float t) {
		return t * t * t;
	}

	inline float EaseOut(float t) {
		 return 1 - pow(1 - t, 3);
	}

}