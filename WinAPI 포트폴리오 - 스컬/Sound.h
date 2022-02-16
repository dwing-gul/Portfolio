#pragma once
#include "Res.h"
#include "SoundManager.h"   

class Sound :
    public Res
{
private :
    LPDIRECTSOUNDBUFFER m_soundBuffer;
    DSBUFFERDESC m_buffInfo;
    int m_volume;

public :
    int Load(const wchar_t* _path);
    void Play(bool _bLoop = false); // 일반 재생
    void PlayToBGM(bool _bLoop = false); // 배경 음악 재생
    void Stop(bool _bReset = false);
    void SetVolume(float _volume); // 볼륨 범위 0 ~ 100
    void SetPosition(float _pos); // 0 to 100
    void SoundRelease();

private :
    bool LoadWaveSound(const wstring& _path);
    int GetDecibel(float _volume);

public :
    Sound();
    virtual ~Sound();
};

