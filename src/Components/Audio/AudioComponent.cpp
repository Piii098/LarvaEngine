#include "LarvaEngine/Components/Audio/AudioComponent.h"
#include "LarvaEngine/Audio/AudioSystem.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Audio/SoundEvent.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"

AudioComponent::AudioComponent(GameObject* parent, int updateOrder)
	: Component(parent, updateOrder) {
	_audioManager = GetParent()->GetScene()->GetManager()->GetGame()->GetAudioManager();
}

AudioComponent::~AudioComponent() {
}

void AudioComponent::Update(float deltaTime) {
	auto iter = _events2D.begin();
	while (iter != _events2D.end())
	{
		if (!iter->IsValid()) {
			iter = _events2D.erase(iter);
		}
		else {
			++iter;
		}
	}

	iter = _events3D.begin();
	while (iter != _events3D.end())
	{
		if (!iter->IsValid()) {
			iter = _events3D.erase(iter);
		}
		else {
			++iter;
		}
	}


}

void AudioComponent::OnUpdateWorldTransform() {
	Matrix4 worldTransform = _parent->WorldTransform();
	for (auto& event : _events3D) {
		if (event.IsValid()) {
			event.Set3DAttributes(worldTransform);
		}
	}
}

SoundEvent AudioComponent::PlayEvent(const std::string& name) {
	SoundEvent event = _parent->GetScene()->GetManager()->GetGame()->GetAudioSystem()->PlayEvent(name);

	if (event.Is3D()) {
		_events3D.emplace_back(event);
	}
	else {
		_events2D.emplace_back(event);
	}

	return event;
}


void AudioComponent::StopAllEvents() {
}

