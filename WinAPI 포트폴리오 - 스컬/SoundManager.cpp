#include "stdafx.h"
#include "SoundManager.h"
#include "Sound.h"
#include "Core.h"

SoundManager::SoundManager() :
    m_sound(nullptr),
    m_BGM(nullptr)
{
}

SoundManager::~SoundManager()
{
}

int SoundManager::init()
{
    if (FAILED(DirectSoundCreate8(NULL, &m_sound, NULL)))
    {
        MessageBox(NULL, L"���� ����̽� ���� ����", L"System Error", MB_OK);
        return false;
    }

    HWND hWnd = Core::GetInst()->GetWindowHandle();
    if (FAILED(m_sound->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE))) // flag �� ����
    {
        MessageBox(NULL, L"���� ����̽� �������� ���� ����", L"System Error", MB_OK);
        return false;
    }

    return true;
}

void SoundManager::RegisterToBGM(Sound* _bgm)
{
    if (nullptr != m_BGM)
    {
        m_BGM->Stop(true);
    }

    m_BGM = _bgm;
}

void SoundManager::ReleaseAllSound()
{
    if (nullptr != m_BGM)
    {
        m_BGM = nullptr;
    }

}
