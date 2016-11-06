
#include "stdafx.h"
#include "Sound.h"

SoundEngine::SoundEngine(){}
SoundEngine::~SoundEngine(){
	Release();
}

void SoundEngine::SoundInit(){
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, 10, FMOD_INIT_NORMAL, NULL);

	//BGM
	FMOD_System_CreateSound(g_System, "..\\..\\Client\\bin\\sound\\sanai.mp3", FMOD_LOOP_NORMAL, 0, &g_Sound[0]);
	//FMOD_System_CreateSound(g_System, "sound\\main.mp3", FMOD_LOOP_NORMAL, 0, &g_Sound[1]);

	//SE
	//FMOD_System_CreateSound(g_System, "sound\\item.wav", FMOD_DEFAULT, 0, &g_Sound[4]);
	//FMOD_System_CreateSound(g_System, "sound\\doorclose.wav", FMOD_DEFAULT, 0, &g_Sound[5]);
	//FMOD_System_CreateSound(g_System, "sound\\doornob1.wav", FMOD_DEFAULT, 0, &g_Sound[6]);

}

void SoundEngine::PlaySE(int soundnum){
	FMOD_BOOL IsPlaying;

	FMOD_Channel_IsPlaying(g_Channel[1], &IsPlaying);
	if (IsPlaying != 1)
		FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[soundnum], 0, &g_Channel[1]);
}

void SoundEngine::PlayMusic(int soundnum){
	FMOD_BOOL IsPlaying;

	FMOD_Channel_IsPlaying(g_Channel[0], &IsPlaying);
	if (IsPlaying != 1)
		FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[soundnum], 0, &g_Channel[0]);
}

void SoundEngine::MusicStop(){
	FMOD_Channel_Stop(g_Channel[0]);
}

void SoundEngine::Release()
{
	FMOD_Sound_Release(g_Sound[0]);
	FMOD_Sound_Release(g_Sound[1]);
	FMOD_System_Close(g_System);
	FMOD_System_Release(g_System);
}