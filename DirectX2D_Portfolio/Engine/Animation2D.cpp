#include "pch.h"
#include "Animation2D.h"
#include "Animator2D.h"
#include "TimeManager.h"
#include "PathManager.h"
#include "ResourceManager.h"
#include "Device.h"
#include "ConstBuffer.h"

Animation2D::Animation2D() :
	m_animator(nullptr),
	m_baseSize(Vector2(1.f, 1.f)),
	m_curFrameIndex(0),
	m_accTime(0.f),
	m_bFinish(false),
	m_bRepeat(true)
{
}

Animation2D::Animation2D(const Animation2D& _origin) :
	m_animator(nullptr),
	m_texture(_origin.m_texture),
	m_baseSize(_origin.m_baseSize),
	m_curFrameIndex(0),
	m_accTime(_origin.m_accTime),
	m_bFinish(false),
	m_bRepeat(true)
{
	for (const auto& frame : _origin.m_vectorAnimFrame)
	{
		m_vectorAnimFrame.push_back(frame);
	}
}

Animation2D::~Animation2D()
{
}

void Animation2D::lateUpdate()
{
	if (m_bFinish)
		return;

	m_accTime += DELTATIME;

	while (m_vectorAnimFrame[m_curFrameIndex].duration < m_accTime)
	{
		m_accTime -= m_vectorAnimFrame[m_curFrameIndex].duration;
		++m_curFrameIndex;

		if (m_curFrameIndex == m_vectorAnimFrame.size())
		{
			m_bFinish = true;
			m_curFrameIndex -= 1;
			break;
		}
	}
}

void Animation2D::UpdateData()
{
	m_texture->UpdateData(SHADER_STAGE::Pixel, 50);

	static const ConstBuffer* CB = Device::GetInst()->GetCB(CONST_BUFFER::ANIM2D_INFO);

	tAnim2DInfo info =
	{
		m_vectorAnimFrame[m_curFrameIndex].leftTop,
		m_vectorAnimFrame[m_curFrameIndex].frameSize,
		m_vectorAnimFrame[m_curFrameIndex].offset,
		m_baseSize,
		true
	};

	CB->SetData(&info);
	CB->UpdateData();
}

void Animation2D::Create(Ptr<Texture> _texture, Vector2 _leftTop, Vector2 _frameSize, Vector2 _baseSize, float _duration, int _frameCount)
{
	assert(_texture.Get());

	m_texture = _texture;

	Vector2 resolution = Vector2(m_texture->Width(), m_texture->Height());

	m_baseSize = Vector2(_baseSize.x / resolution.x, _baseSize.y / resolution.y);

	tAnimFrame frame = {};
	frame.frameSize.x = _frameSize.x / resolution.x;
	frame.frameSize.y = _frameSize.y / resolution.y;
	frame.duration = _duration;

	Vector2 leftTop = Vector2(_leftTop.x / resolution.x, _leftTop.y / resolution.y);

	for (int i = 0; i < _frameCount; ++i)
	{
		frame.leftTop = Vector2(leftTop.x + (float)i * frame.frameSize.x, leftTop.y);
		m_vectorAnimFrame.push_back(frame);
	}
}

void Animation2D::Save(const wstring& _relativePath, const wstring& _name)
{
	wstring filePath = PathManager::GetInst()->GetContentPath();
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
	fwprintf(file, L"%d\n", (int)m_vectorAnimFrame.size());

	for (size_t i = 0; i < m_vectorAnimFrame.size(); ++i)
	{
		fwprintf(file, L"LEFT_TOP\n");
		fwprintf(file, L"%f %f\n", m_vectorAnimFrame[i].leftTop.x, m_vectorAnimFrame[i].leftTop.y);

		fwprintf(file, L"FRAME_SIZE\n");
		fwprintf(file, L"%f %f\n", m_vectorAnimFrame[i].frameSize.x, m_vectorAnimFrame[i].frameSize.y);

		fwprintf(file, L"OFFSET\n");
		fwprintf(file, L"%f %f\n", m_vectorAnimFrame[i].offset.x, m_vectorAnimFrame[i].offset.y);

		fwprintf(file, L"DURATION\n");
		fwprintf(file, L"%f\n", m_vectorAnimFrame[i].duration);
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

void Animation2D::Load(const wstring& _relativePath)
{
	wstring filePath = PathManager::GetInst()->GetContentPath();
	filePath += _relativePath;

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

		m_vectorAnimFrame.push_back(frame);
	}

	fwscanf_s(file, L"%s", buffer, 255);
	fwscanf_s(file, L"%s", buffer, 255);
	m_texture = ResourceManager::GetInst()->FindRes<Texture>(buffer);
	wstring key = buffer;

	fwscanf_s(file, L"%s", buffer, 255);
	fwscanf_s(file, L"%s", buffer, 255);

	if (nullptr == m_texture)
	{
		m_texture = ResourceManager::GetInst()->Load<Texture>(key, buffer);
	}

	fclose(file);
}

void Animation2D::SaveToScene(FILE* _file)
{
	Entity::SaveToScene(_file);
	SaveResourceToFile(m_texture, _file);

	UINT frameCount = (UINT)m_vectorAnimFrame.size();
	SaveToFile(&frameCount, _file);

	for (size_t i = 0; i < m_vectorAnimFrame.size(); ++i)
	{
		SaveToFile(&m_vectorAnimFrame[i], _file);
	}

	SaveToFile(&m_baseSize, _file);
	SaveToFile(&m_curFrameIndex, _file);
}

void Animation2D::LoadFromScene(FILE* _file)
{
	Entity::LoadFromScene(_file);
	LoadResourceFromFile(m_texture, _file);

	UINT frameCount = 0;
	LoadFromFile(&frameCount, _file);

	for (UINT i = 0; i < frameCount; ++i)
	{
		tAnimFrame frame = {};
		LoadFromFile(&frame, _file);
		m_vectorAnimFrame.push_back(frame);
	}

	LoadFromFile(&m_baseSize, _file);
	LoadFromFile(&m_curFrameIndex, _file);
}