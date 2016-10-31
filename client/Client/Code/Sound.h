#pragma once

#ifndef Sound_h__
#define Sound_h__

#include "fmod.hpp"
#pragma comment (lib, "fmodex_vc.lib")
using namespace FMOD;

class SoundEngine
{
	FMOD_SYSTEM *g_System;
	FMOD_SOUND *g_Sound[10];
	FMOD_CHANNEL *g_Channel[2];
public:
	SoundEngine();
	~SoundEngine();
	void SoundInit();
	void PlaySE(int);
	void PlayMusic(int);
	void MusicStop();
	void Release();
};

#endif