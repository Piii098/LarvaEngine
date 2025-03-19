#include "LarvaEngine/Audio/SoundEvent.h"
#include "LarvaEngine/Audio/AudioSystem.h"
#include <fmod_studio.hpp>

#pragma region コンストラクタ:デストラクタ

SoundEvent::SoundEvent(class AudioSystem* system, unsigned int id)
	: mSystem(system)
	, mID(id)
{
}

SoundEvent::SoundEvent()
	:mSystem(nullptr)
	, mID(0)
{
}

#pragma endregion

#pragma region パブリック関数


bool SoundEvent::IsValid()
{
	return (mSystem && mSystem->GetEventInstance(mID) != nullptr);
}

void SoundEvent::Restart()
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->start();
	}
}

void SoundEvent::Stop(bool allowFadeOut /* true */)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		FMOD_STUDIO_STOP_MODE mode = allowFadeOut ?
			FMOD_STUDIO_STOP_ALLOWFADEOUT :
			FMOD_STUDIO_STOP_IMMEDIATE;
		event->stop(mode);
	}
}

void SoundEvent::SetPaused(bool pause)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setPaused(pause);
	}
}

void SoundEvent::SetVolume(float value)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setVolume(value);
	}
}

void SoundEvent::SetPitch(float value)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setPitch(value);
	}
}

void SoundEvent::SetParameter(const std::string& name, float value)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setParameterByName(name.c_str(), value);
		//event->setParameterValue(name.c_str(), value);
	}
}

bool SoundEvent::GetPaused() const
{
	bool retVal = false;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->getPaused(&retVal);
	}
	return retVal;
}

float SoundEvent::GetVolume() const
{
	float retVal = 0.0f;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->getVolume(&retVal);
	}
	return retVal;
}

float SoundEvent::GetPitch() const
{
	float retVal = 0.0f;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->getPitch(&retVal);
	}
	return retVal;
}

float SoundEvent::GetParameter(const std::string& name)
{
	float retVal = 0.0f;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->getParameterByName(name.c_str(), &retVal);
		//event->getParameterValueByName(name.c_str(), &retVal);
	}
	return retVal;
}

bool SoundEvent::Is3D() const
{
	bool retVal = false;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		FMOD::Studio::EventDescription* desc = nullptr;
		event->getDescription(&desc);
		if (desc)
		{
			desc->is3D(&retVal);
		}
	}
	return retVal;
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

void SoundEvent::Set3DAttributes(const Matrix4& worldTrans)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		FMOD_3D_ATTRIBUTES attr;
		// Set position, forward, up
		attr.position = VecToFMOD(worldTrans.GetTranslation());
		// In world transform, first row is forward
		attr.forward = VecToFMOD(worldTrans.GetXAxis());
		// Third row is up
		attr.up = VecToFMOD(worldTrans.GetZAxis());
		// Set velocity to zero (fix if using Doppler effect)
		attr.velocity = { 0.0f, 0.0f, 0.0f };
		event->set3DAttributes(&attr);
	}
}



#pragma endregion
