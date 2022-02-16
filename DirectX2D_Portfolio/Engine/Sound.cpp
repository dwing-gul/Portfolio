#include "pch.h"
#include "Sound.h"
#include "ResourceManager.h"

FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* _channelControl, FMOD_CHANNELCONTROL_TYPE _controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE _callbackType, void* _commandData1, void* _commandData2);
FMOD::System* Sound::g_FMOD = nullptr;

Sound::Sound() :
    Res(RESOURCE_TYPE::SOUND),
    m_sound(nullptr)
{
}

Sound::~Sound()
{
    if (nullptr != m_sound)
    {
        m_sound->release();
        m_sound = nullptr;
    }
}

int Sound::Play(int _roopCount, float _volume, bool _bOverlap)
{
    if (_roopCount <= -1)
    {
        assert(nullptr);
    }

    if (!_bOverlap && !m_listChannel.empty())
    {
        return -1;
    }

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

void Sound::Stop()
{
    list<FMOD::Channel*>::iterator iter;

    while (!m_listChannel.empty())
    {
        iter = m_listChannel.begin();
        (*iter)->stop();
    }
}

void Sound::SetVolume(float _volume, int _channelIndex)
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

bool Sound::IsPlaying(int _channelIndex)
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

void Sound::RemoveChannel(FMOD::Channel* _targetChannel)
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

int Sound::Load(const wstring& _filePath)
{
    string path = WStringToString(_filePath);

    if (FMOD_OK != g_FMOD->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_sound))
    {
        assert(nullptr);
    }

    return S_OK;
}

FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* _channelControl, FMOD_CHANNELCONTROL_TYPE _controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE _callbackType, void* _commandData1, void* _commandData2)
{
    FMOD::Channel* cppChannel = (FMOD::Channel*)_channelControl;
    Sound* sound = nullptr;

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