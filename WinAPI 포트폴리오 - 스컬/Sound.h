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
    void Play(bool _bLoop = false); // �Ϲ� ���
    void PlayToBGM(bool _bLoop = false); // ��� ���� ���
    void Stop(bool _bReset = false);
    void SetVolume(float _volume); // ���� ���� 0 ~ 100
    void SetPosition(float _pos); // 0 to 100
    void SoundRelease();

private :
    bool LoadWaveSound(const wstring& _path);
    int GetDecibel(float _volume);

public :
    Sound();
    virtual ~Sound();
};

