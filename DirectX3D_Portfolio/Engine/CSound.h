#pragma once
#include "CRes.h"

#include "FMOD/fmod.h"
#include "FMOD/fmod.hpp"
#include "FMOD/fmod_codec.h"

#ifdef _DEBUG
#pragma comment(lib, "FMOD/x64/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/x64/fmod64_vc.lib")
#endif

class CSound :
    public CRes
{
public:
    static FMOD::System*    g_FMOD;

private:
    FMOD::Sound*            m_sound;
    list<FMOD::Channel*>    m_listChannel;

    bool                    m_bLoop;

public:
    int Play(int _roopCount, float _volume = 1.f, bool _bOverlap = false); // roop - 0 (무한반복), 0 ~ 1(Volume)
    void Stop();

    void SetVolume(float _volume, int _channelIndex);
    bool IsPlaying(int _channelIndex);
    bool IsLoop() { return m_bLoop; }

private:
    void RemoveChannel(FMOD::Channel* _targetChannel);
    friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* _channelControl, FMOD_CHANNELCONTROL_TYPE _controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE _callbackType, void* _commandData1, void* _commandData2);

public:
    virtual void Save(const wstring& _relativePath) {}
    virtual int Load(const wstring& _filePath);

    CLONE_DISABLE(CSound)

public:
    CSound();
    virtual ~CSound();
};

