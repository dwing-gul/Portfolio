#include "pch.h"
#include "CAnimation2D.h"
#include "CAnimator2D.h"
#include "CTimeManager.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CAnimation2D::CAnimation2D() :
	m_animator(nullptr),
	m_texture(nullptr),
	m_baseSize(Vec2(1.f, 1.f)),
	m_curFrameIndex(0),
	m_accTime(0.f),
	m_bFinish(false),
	m_bRepeat(true)
{
}

CAnimation2D::CAnimation2D(const CAnimation2D& _origin) :
	m_animator(nullptr),
	m_texture(_origin.m_texture),
	m_baseSize(_origin.m_baseSize),
	m_curFrameIndex(0),
	m_accTime(_origin.m_accTime),
	m_bFinish(false),
	m_bRepeat(_origin.m_bRepeat)
{
	for (auto& frame : _origin.m_vecAnimFrame)
	{
		m_vecAnimFrame.push_back(frame);
	}
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::lateUpdate()
{
	if (m_bFinish)
		return;

	m_accTime += DELTATIME;

	while (m_vecAnimFrame[m_curFrameIndex].duration < m_accTime)
	{
		m_accTime -= m_vecAnimFrame[m_curFrameIndex].duration;
		++m_curFrameIndex;

		if (m_curFrameIndex == m_vecAnimFrame.size())
		{
			m_bFinish = true;
			m_curFrameIndex -= 1;
			break;
		}
	}
}

void CAnimation2D::UpdateData()
{
	m_texture->UpdateData(SHADER_STAGE::Pixel, 50);

	static const CConstBuffer* buffer = CDevice::GetInst()->GetCB(CONST_BUFFER::ANIM2D_INFO);

	tAnim2DInfo info =
	{
		m_vecAnimFrame[m_curFrameIndex].leftTop,
		m_vecAnimFrame[m_curFrameIndex].frameSize,
		m_vecAnimFrame[m_curFrameIndex].offset,
		m_baseSize, true
	};

	buffer->SetData(&info);
	buffer->UpdateData();
}

void CAnimation2D::Create(Ptr<CTexture> _texture, Vec2 _leftTop, Vec2 _frameSize, Vec2 _baseSize, float _duration, int _frameCount)
{
	assert(_texture.Get());

	m_texture = _texture;

	Vec2 resolution = Vec2(m_texture->Width(), m_texture->Height());

	m_baseSize = Vec2(_baseSize.x / resolution.x, _baseSize.y / resolution.y);

	tAnimFrame frame = {};
	frame.frameSize.x = _frameSize.x / resolution.x;
	frame.frameSize.y = _frameSize.y / resolution.y;
	frame.duration = _duration;

	Vec2 leftTop = Vec2(_leftTop.x / resolution.x, _leftTop.y / resolution.y);

	for (int i = 0; i < _frameCount; ++i)
	{
		frame.leftTop = Vec2(leftTop.x + (float)i * frame.frameSize.x, leftTop.y);
		m_vecAnimFrame.push_back(frame);
	}
}

void CAnimation2D::Create_Tool(Ptr<CTexture> _pTex, vector<tAnimFrame>& _vecAnimFrm)
{
	assert(_pTex.Get());

	m_texture = _pTex;
	Vec2 vResolution = Vec2(m_texture->Width(), m_texture->Height());

	for (int i = 0; i < _vecAnimFrm.size(); ++i)
	{
		m_vecAnimFrame.push_back(_vecAnimFrm[i]);
	}

	m_baseSize = Vec2(150.f / vResolution.x, 150.f / vResolution.y);
}

void CAnimation2D::Save(const wstring& _relativePath, const wstring& _name)
{
	wstring filePath = CPathManager::GetInst()->GetContentPath();
	filePath += _relativePath;
	filePath += _name;

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"w");
	assert(file);

	fwprintf(file, L"ANIMATION_NAME\n");
	fwprintf(file, GetName().c_str());
	fwprintf(file, L"\n");

	fwprintf(file, L"REPEAT\n");
	fwprintf(file, L"%d\n", (int)m_bRepeat);

	fwprintf(file, L"BASE_SIZE\n");
	fwprintf(file, L"%f %f\n", m_baseSize.x, m_baseSize.y);

	fwprintf(file, L"FRAME_COUNT\n");
	fwprintf(file, L"%d\n", (int)m_vecAnimFrame.size());

	for (size_t i = 0; i < m_vecAnimFrame.size(); ++i)
	{
		fwprintf(file, L"LEFT_TOP\n");
		fwprintf(file, L"%f %f\n", m_vecAnimFrame[i].leftTop.x, m_vecAnimFrame[i].leftTop.y);

		fwprintf(file, L"FRAME_SIZE\n");
		fwprintf(file, L"%f %f\n", m_vecAnimFrame[i].frameSize.x, m_vecAnimFrame[i].frameSize.y);

		fwprintf(file, L"OFFSET\n");
		fwprintf(file, L"%f %f\n", m_vecAnimFrame[i].offset.x, m_vecAnimFrame[i].offset.y);

		fwprintf(file, L"DURATION\n");
		fwprintf(file, L"%f\n", m_vecAnimFrame[i].duration);
		fwprintf(file, L"\n");
	}

	fwprintf(file, L"TEXTURE_NAME\n");
	fwprintf(file, m_texture->GetKey().c_str());
	fwprintf(file, L"\n");

	fwprintf(file, L"TEXTURE_PATH\n");
	fwprintf(file, m_texture->GetRelativePath().c_str());
	fwprintf(file, L"\n");

	fclose(file);
}

void CAnimation2D::Load(const wstring& _relativeFileName)
{
	wstring filePath = CPathManager::GetInst()->GetContentPath();
	filePath += _relativeFileName;

	wchar_t buffer[255] = {};

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"r");
	assert(file);

	fwscanf_s(file, L"%s", buffer, 255);
	fwscanf_s(file, L"%s", buffer, 255);

	SetName(buffer);

	fwscanf_s(file, L"%s", buffer, 255);
	fwscanf_s(file, L"%d", (int*)&m_bRepeat);

	fwscanf_s(file, L"%s", buffer, 255);
	fwscanf_s(file, L"%f", &m_baseSize.x);
	fwscanf_s(file, L"%f", &m_baseSize.y);

	fwscanf_s(file, L"%s", buffer, 255);
	int frameCount = 0;
	fwscanf_s(file, L"%d", &frameCount);

	tAnimFrame frame = {};
	for (int i = 0; i < frameCount; ++i)
	{
		fwscanf_s(file, L"%s", buffer, 255);
		fwscanf_s(file, L"%f", &frame.leftTop.x);
		fwscanf_s(file, L"%f", &frame.leftTop.y);

		fwscanf_s(file, L"%s", buffer, 255);
		fwscanf_s(file, L"%f", &frame.frameSize.x);
		fwscanf_s(file, L"%f", &frame.frameSize.y);

		fwscanf_s(file, L"%s", buffer, 255);
		fwscanf_s(file, L"%f", &frame.offset.x);
		fwscanf_s(file, L"%f", &frame.offset.y);

		fwscanf_s(file, L"%s", buffer, 255);
		fwscanf_s(file, L"%f", &frame.duration);

		m_vecAnimFrame.push_back(frame);
	}

	fwscanf_s(file, L"%s", buffer, 255);
	fwscanf_s(file, L"%s", buffer, 255);
	m_texture = CResourceManager::GetInst()->FindRes<CTexture>(buffer);
	wstring key = buffer;

	fwscanf_s(file, L"%s", buffer, 255);
	fwscanf_s(file, L"%s", buffer, 255);

	if (nullptr == m_texture)
	{
		m_texture = CResourceManager::GetInst()->Load<CTexture>(key, buffer);
	}

	fclose(file);
}

void CAnimation2D::SaveToScene(FILE* _file)
{
	CEntity::SaveToScene(_file);
	SaveResourceToFile(m_texture, _file);

	UINT frameCount = (UINT)m_vecAnimFrame.size();
	SaveToFile(&frameCount, _file);

	for (size_t i = 0; i < m_vecAnimFrame.size(); ++i)
	{
		SaveToFile(&m_vecAnimFrame[i], _file);
	}

	SaveToFile(&m_baseSize, _file);
	SaveToFile(&m_curFrameIndex, _file);
}

void CAnimation2D::LoadFromScene(FILE* _file)
{
	CEntity::LoadFromScene(_file);
	LoadResourceFromFile(m_texture, _file);

	UINT frameCount = 0;
	LoadFromFile(&frameCount, _file);

	for (UINT i = 0; i < frameCount; ++i)
	{
		tAnimFrame frame = {};
		LoadFromFile(&frame, _file);
		m_vecAnimFrame.push_back(frame);
	}

	LoadFromFile(&m_baseSize, _file);
	LoadFromFile(&m_curFrameIndex, _file);
}
