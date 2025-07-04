﻿#pragma once
#include <string>
#include "LarvaEngine/Core/Utilities/Math.h"

/*
EventInstanceのラッパークラス
IDを持ち、FMODのイベントを再生、停止、パラメータの設定などを行う
*/

class SoundEvent
{
public:
	SoundEvent();
	// Returns true if associated FMOD event still exists
	bool IsValid() ;
	// Restart event from beginning
	void Restart();
	// Stop this event
	void Stop(bool allowFadeOut = true);
	// Setters
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);
	// Getters
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);
	// Positional
	bool Is3D() const;
	void Set3DAttributes(const Matrix4& worldTrans);

protected:
	// Make this constructor protected and AudioSystem a friend
	// so that only AudioSystem can access this constructor.
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);
private:
	class AudioSystem* mSystem;
	unsigned int mID;
};