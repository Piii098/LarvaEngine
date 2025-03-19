#pragma once
#include <string>
#include "LarvaEngine/Core/GameObject.h"

class Scene;
class AnimationComponent;
class LightComponent;

class Particle : public GameObject {
public:
	Particle(class Scene* scene, int bufferLayer = 10);
	~Particle();

	void SetTexture(const std::string& name);
	void SetSpriteSheet(int frameWidth, int frameHeight, int startX, 
		int startY, int frameCount,
		float frameDuration, bool isLooping);
	void SetLight(Vector3 color, float intensity, float range, Vector2 offset = Vector2::Zero);

private:
	AnimationComponent* _animation;
	LightComponent* _light;
	int _bufferLayer;
};