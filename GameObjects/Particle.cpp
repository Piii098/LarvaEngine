#include "GameObjects/Particle.h"
#include "Components/Draw/AnimationComponent.h"

Particle::Particle(Scene* scene)
	: GameObject(scene){
	Scale(1.f);
	_animation = new AnimationComponent(this, 10, 0);
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