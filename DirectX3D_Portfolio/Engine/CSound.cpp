#include "pch.h"
#include "CSound.h"
#include "CResourceManager.h"

FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* _channelControl, FMOD_CHANNELCONTROL_TYPE _controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE _callbackType, void* _commandData1, void* _commandData2);
FMOD::System* CSound::g_FMOD = nullptr;

CSound::CSound() :
    CRes(RESOURCE_TYPE::SOUND),
    m_sound(nullptr),
    m_bLoop(false)
{
}

CSound::~CSound()
{
    if (nullptr != m_sound)
    {
        m_sound->release();
        m_sound = nullptr;
    }
}

int CSound::Play(int _roopCount, float _volume, bool _bOverlap)
{
    if (_roopCount <= -1)
    {
        assert(nullptr);
    }

    if (!_bOverlap && !m_listChannel.empty())
    {
        return -1;
    }

    if (_roopCount == 0)
        m_bLoop = true;

    _roopCount -= 1;

    FMOD::Channel* channel = nullptr;
    g_FMOD->playSound(m_sound, nullptr, false, &channel);
    channel->setVolume(_volume);

    channel->setCallback(CHANNEL_CALLBACK);
    channel->setUserData(this);

    channel->setMode(FMOD_LOOP_NORMAL);
    channel->setLoopCount(_roopCount);

    m_listChannel.push_back(channel);

    int index = -1;
    channel->getIndex(&index);

    return index;
}

void CSound::Stop()
{
    list<FMOD::Channel*>::iterator iter;

    while (!m_listChannel.empty())
    {
        iter = m_listChannel.begin();
        (*iter)->stop();
    }

    m_bLoop = false;
}

void CSound::SetVolume(float _volume, int _channelIndex)
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    int index = -1;
    for (; iter != m_listChannel.end(); ++iter)
    {
        (*iter)->getIndex(&index);
        if (_channelIndex == index)
        {
            (*iter)->setVolume(_volume);
            return;
        }
    }
}

bool CSound::IsPlaying(int _channelIndex)
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    int index = -1;
    bool bPlay = false;

    for (; iter != m_listChannel.end(); ++iter)
    {
        (*iter)->getIndex(&index);
        if (_channelIndex == index)
        {
            (*iter)->isPlaying(&bPlay);

            return bPlay;
        }
    }

    return bPlay;
}

void CSound::RemoveChannel(FMOD::Channel* _targetChannel)
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    for (; iter != m_listChannel.end(); ++iter)
    {
        if (*iter == _targetChannel)
        {
            m_listChannel.erase(iter);
            return;
        }
    }
}

int CSound::Load(const wstring& _filePath)
{
    string path(_filePath.begin(), _filePath.end());

    if (FMOD_OK != g_FMOD->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_sound))
    {
        assert(nullptr);
    }

    return S_OK;
}

FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* _channelControl, FMOD_CHANNELCONTROL_TYPE _controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE _callbackType, void* _commandData1, void* _commandData2)
{
    FMOD::Channel* cppChannel = (FMOD::Channel*)_channelControl;
    CSound* sound = nullptr;

    switch (_controlType)
    {
    case FMOD_CHANNELCONTROL_CALLBACK_END:
    {
        cppChannel->getUserData((void**)&sound);
        sound->RemoveChannel(cppChannel);
    }
    break;
    }

    return FMOD_OK;
}