#pragma once
#include <unordered_map>
#include <string>
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Audio/SoundEvent.h"

namespace FMOD{
	class System;
	//void Debug_Initialize();
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};

class Game;

class AudioSystem {
public:
	AudioSystem(Game* game);
	~AudioSystem();

	bool Initialize();
	void Shutdown();

	void Update(float deltaTime);

	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();
	
	SoundEvent PlayEvent(const std::string& name);

	void SetListener(const class Matrix4& viewMatrix);

	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);

protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:

	static unsigned int s_nextID;

	Game* _game;
	FMOD::Studio::System* _studioSystem;
	FMOD::System* _coreSystem;
	std::unordered_map<std::string, FMOD::Studio::Bank*> _banks;
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> _events;
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> _eventInstances;

	std::unordered_map<std::string, FMOD::Studio::Bus*> _buses;
};