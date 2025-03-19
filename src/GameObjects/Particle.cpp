#include "LarvaEngine/GameObjects/Particle.h"
#include "LarvaEngine/Components/Draw/AnimationComponent.h"
#include "LarvaEngine/Components/Light/LightComponent.h"

Particle::Particle(Scene* scene, int bufferLayer)
	: GameObject(scene)
	, _bufferLayer(bufferLayer){
	Scale(1.f);
	_animation = new AnimationComponent(this, bufferLayer);
}

Particle::~Particle() {

}

void Particle::SetTexture(const std::string& name) {
	
	_animation->SetTexture(name);

}

void Particle::SetSpriteSheet(int frameWidth, int frameHeight, int startX, int startY, int frameCount, float frameDuration, bool isLooping) {
	_animation->AddAnimationFromSpriteSheet("normal", frameWidth, frameHeight, startX, startY, frameCount, frameDuration, isLooping);
	_animation->Play("normal");
}

void Particle::SetLight(Vector3 color, float intensity, float range, Vector2 offset) {
	_light = new LightComponent(this, _bufferLayer);
	_light->LightColor(color);
	_light->LightIntensity(intensity);
	_light->LightRange(range);
	_light->LightOffset(offset);
}