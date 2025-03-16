#pragma once
#include "Components/Component.h"
#include <string>
#include <vector>

class SoundEvent;

class AudioComponent : public Component {
public:
	AudioComponent(class GameObject* parent, int updateOrder = 200);
	~AudioComponent();

	void Update(float deltaTime) override;	
	void OnUpdateWorldTransform() override;

	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvents();
private:
	std::vector<SoundEvent> _events2D;
	std::vector<SoundEvent> _events3D;
};