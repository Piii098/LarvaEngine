#pragma once
#include <string>
#include "GameObjects/GameObject.h"

class Scene;
class AnimationComponent;

class Particle : public GameObject {
public:
	Particle(class Scene* scene);
	~Particle();

	void SetTexture(const std::string& name);
	void SetSpriteSheet(int frameWidth, int frameHeight, int startX, 
		int startY, int frameCount,
		float frameDuration, bool isLooping);

private:
	AnimationComponent* _animation;
};