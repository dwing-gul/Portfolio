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
	LPDIRECTSOUND8 m_sound; // ���� ī�� ��ǥ ��ü
	Sound* m_BGM; // ���� ��� ����

public :
	int init();
	LPDIRECTSOUND8 GetSoundDevice() { return m_sound; }
	void RegisterToBGM(Sound* _bgm);
	void ReleaseAllSound();
};

