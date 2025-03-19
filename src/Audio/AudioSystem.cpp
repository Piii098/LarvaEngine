#include "LarvaEngine/Audio/AudioSystem.h"
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <SDL3/SDL.h>
#include <vector>

#pragma region コンストラクタ:デストラクタ

unsigned int AudioSystem::s_nextID = 0;

AudioSystem::AudioSystem(Game* game)
	: _game(game), _studioSystem(nullptr), _coreSystem(nullptr) {
}

AudioSystem::~AudioSystem() {

}

#pragma endregion

#pragma region パブリック関数

bool AudioSystem::Initialize() {

	FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR, FMOD_DEBUG_MODE_TTY);

	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&_studioSystem);
	if (result != FMOD_OK) {
		SDL_Log("Failed to create FMOD Studio System: %s", FMOD_ErrorString(result));
		return false;
	}

	result = _studioSystem->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK) {
		SDL_Log("Failed to initialize FMOD Studio System: %s", FMOD_ErrorString(result));
		return false;
	}

	_studioSystem->getCoreSystem(&_coreSystem);

	// Load the master banks (strings first)
	LoadBank("Assets/Master.strings.bank");
	LoadBank("Assets/Master.bank");

	return true;

}

void AudioSystem::Shutdown() {
	UnloadAllBanks();
	if (_studioSystem) {
		_studioSystem->release();
	}
}

void AudioSystem::LoadBank(const std::string& name) {
	if (_banks.find(name) != _banks.end()) {
		return;
	}

	FMOD::Studio::Bank* bank = nullptr; // バンクをロード
	FMOD_RESULT result = _studioSystem->loadBankFile(name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);	

	const int maxpathlength = 512;
	if (result == FMOD_OK) {
		_banks.emplace(name, bank); // バンクを登録
		bank->loadSampleData(); // サンプルデータをロード
		int numEvents = 0;
		bank->getEventCount(&numEvents); // イベントの数を取得
		
		if (numEvents > 0) {
			// イベントのリストを取得
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxpathlength];
			for (int i = 0; i < numEvents; i++) {
				FMOD::Studio::EventDescription* event = events[i];
				event->getPath(eventName, maxpathlength, nullptr);
				_events.emplace(eventName, event);
			}
		}

		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			// Get list of buses in this bank
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				// Get the path of this bus (like bus:/SFX)
				bus->getPath(busName, 512, nullptr);
				// Add to buses map
				_buses.emplace(busName, bus);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name) {
	auto iter = _banks.find(name);
	if (iter == _banks.end()) {
		return;
	}


	// First we need to remove all events from this bank
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		// Get event descriptions for this bank
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		// Get list of events
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			// Get the path of this event
			e->getPath(eventName, 512, nullptr);
			// Remove this event
			auto eventi = _events.find(eventName);
			if (eventi != _events.end())
			{
				_events.erase(eventi);
			}
		}
	}

	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0)
	{
		// Get list of buses in this bank
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			// Get the path of this bus (like bus:/SFX)
			bus->getPath(busName, 512, nullptr);
			// Remove this bus
			auto busi = _buses.find(busName);
			if (busi != _buses.end())
			{
				_buses.erase(busi);
			}
		}
	}

	// Unload sample data and bank
	bank->unloadSampleData();
	bank->unload();

}

void AudioSystem::UnloadAllBanks() {
	for (auto& bank : _banks) {
		bank.second->unloadSampleData();
		bank.second->unload();
	}
	_banks.clear();
	_events.clear();
}

void AudioSystem::Update(float deltaTime) {
	
	std::vector<unsigned int> done; // 終了したイベントのIDを格納するベクター
	
	for (auto& event : _eventInstances) { // 全てのイベントをチェック
		FMOD::Studio::EventInstance* e = event.second;
		FMOD_STUDIO_PLAYBACK_STATE state;
		event.second->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED) { // 再生が終了したら
			event.second->release();
			done.emplace_back(event.first); // 終了したイベントのIDを格納
		}
	}

	for (auto id : done) { // 終了したイベントを削除
		_eventInstances.erase(id);
	}

	_studioSystem->update();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name) {
	unsigned int retID = 0;
	auto iter = _events.find(name);
	if (iter != _events.end()) {
		FMOD::Studio::EventInstance* eventInst = nullptr;
		iter->second->createInstance(&eventInst);
		if (eventInst) {
			eventInst->start();
			
			s_nextID++;
			retID = s_nextID;

			_eventInstances.emplace(retID, eventInst);
		}
	}
	return SoundEvent(this, retID);
}


namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		// Convert from our coordinates (+x forward, +y right, +z up)
		// to FMOD (+z forward, +x right, +y up)
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
	// Invert the view matrix to get the correct vectors
	Matrix4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	// Set position, forward, up
	listener.position = VecToFMOD(invView.GetTranslation());
	// In the inverted view, third row is forward
	listener.forward = VecToFMOD(invView.GetZAxis());
	// In the inverted view, second row is up
	listener.up = VecToFMOD(invView.GetYAxis());
	// Set velocity to zero (fix if using Doppler effect)
	listener.velocity = { 0.0f, 0.0f, 0.0f };
	// Send to FMOD
	_studioSystem->setListenerAttributes(0, &listener);
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = _buses.find(name);
	if (iter != _buses.end())
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string& name) const
{
	bool retVal = false;
	const auto iter = _buses.find(name);
	if (iter != _buses.end())
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	auto iter = _buses.find(name);
	if (iter != _buses.end())
	{
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause)
{
	auto iter = _buses.find(name);
	if (iter != _buses.end())
	{
		iter->second->setPaused(pause);
	}
}


#pragma endregion

#pragma region プライベート関数


FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = _eventInstances.find(id);
	if (iter != _eventInstances.end())
	{
		event = iter->second;
	}
	return event;
}


#pragma endregion
