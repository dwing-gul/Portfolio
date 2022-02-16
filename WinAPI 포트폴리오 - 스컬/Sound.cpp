#include "stdafx.h"
#include "Sound.h"
#include "SoundManager.h"

Sound::Sound() :
	m_soundBuffer(nullptr),
	m_buffInfo{},
	m_volume(0)
{
}

Sound::~Sound()
{
	if (nullptr != m_soundBuffer)
	{
		m_soundBuffer->Release();
	}
}

int Sound::Load(const wchar_t* _path)
{
	if (nullptr == SoundManager::GetInst()->GetSoundDevice())
		assert(nullptr);

	wchar_t ext[10] = { 0 };
	_wsplitpath_s(_path, nullptr, 0, nullptr, 0, nullptr, 0, ext, 10);

	if (!wcscmp(ext, L".wav"))
	{
		if (false == LoadWaveSound(_path))
		{
			assert(nullptr);
		}
	}
	else 
		assert(nullptr);

	return S_OK;
}

bool Sound::LoadWaveSound(const wstring& _path)
{
	HMMIO file;
	wstring filePath = _path;

	file = mmioOpen((wchar_t*)filePath.c_str(), NULL, MMIO_READ);

	if (nullptr == file)
	{
		MessageBox(NULL, L"사운드 리소스 경로에 파일 없음", L"사운드 로딩 실패", MB_OK);
		return false;
	}

	MMCKINFO parent;
	memset(&parent, 0, sizeof(parent));
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmioDescend(file, &parent, NULL, MMIO_FINDRIFF);

	MMCKINFO child;
	memset(&child, 0, sizeof(child));
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(file, &child, &parent, MMIO_FINDCHUNK);

	WAVEFORMATEX wft;
	memset(&wft, 0, sizeof(wft));
	mmioRead(file, (char*)&wft, sizeof(wft));

	mmioAscend(file, &child, 0);
	child.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(file, &child, &parent, MMIO_FINDCHUNK);

	memset(&m_buffInfo, 0, sizeof(DSBUFFERDESC));
	m_buffInfo.dwBufferBytes = child.cksize;
	m_buffInfo.dwSize = sizeof(DSBUFFERDESC);
	m_buffInfo.dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLVOLUME;
	m_buffInfo.lpwfxFormat = &wft;

	if (FAILED(SoundManager::GetInst()->GetSoundDevice()->CreateSoundBuffer(&m_buffInfo, &m_soundBuffer, NULL)))
	{
		MessageBox(NULL, L"사운드버퍼생성실패", L"", MB_OK);
		return false;
	}

	void* write1 = NULL;
	void* write2 = NULL;
	DWORD length1, length2;

	m_soundBuffer->Lock(0, child.cksize, &write1, &length1, &write2, &length2, 0L);

	if (write1 > 0)
	{
		mmioRead(file, (char*)write1, length1);
	}
	if (write2 > 0)
	{
		mmioRead(file, (char*)write2, length2);
	}

	m_soundBuffer->Unlock(write1, length1, write2, length2);

	mmioClose(file, 0);

	SetVolume(50.f);

	return true;
}

void Sound::Play(bool _bLoop)
{
	if (_bLoop)
	{
		m_soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	}
	else
	{
		m_soundBuffer->Play(0, 0, 0);
	}
}

void Sound::PlayToBGM(bool _bLoop)
{
	SoundManager::GetInst()->RegisterToBGM(this);

	if (_bLoop)
	{
		m_soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	}
	else
	{
		m_soundBuffer->Play(0, 0, 0);
	}
}

void Sound::Stop(bool _bReset)
{
	m_soundBuffer->Stop();

	if (_bReset)
	{
		m_soundBuffer->SetCurrentPosition(0);
	}
}

void Sound::SetVolume(float _volume)
{
	m_volume = GetDecibel(_volume);
	m_soundBuffer->SetVolume(m_volume);
}

void Sound::SetPosition(float _pos)
{
	Stop(true);

	DWORD bytes = (DWORD)((_pos / 100.f) * (float)m_buffInfo.dwBufferBytes);
	m_soundBuffer->SetCurrentPosition(bytes);

	Play();
}

void Sound::SoundRelease()
{
	if (nullptr != m_soundBuffer)
	{
		m_soundBuffer->Release();
	}

	SoundManager::GetInst()->ReleaseAllSound();
}

int Sound::GetDecibel(float _volume)
{
	if (_volume > 100.f)
		_volume = 100.f;
	else if (_volume <= 0.f)
		_volume = 0.00001f;

	int volume = (LONG)(-2000.0 * log10(100.f / _volume));

	if (volume < -10000)
		volume = -10000;

	return volume;
}
