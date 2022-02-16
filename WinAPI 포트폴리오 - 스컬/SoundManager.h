#pragma once

#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

class Sound;

class SoundManager
{
	SINGLE(SoundManager)
private :
	LPDIRECTSOUND8 m_sound; // 사운드 카드 대표 객체
	Sound* m_BGM; // 현재 배경 음악

public :
	int init();
	LPDIRECTSOUND8 GetSoundDevice() { return m_sound; }
	void RegisterToBGM(Sound* _bgm);
	void ReleaseAllSound();
};

